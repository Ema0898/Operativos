#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
#include <structs.h>
#include <ppm.h>
#include <process.h>
#include <print.h>

int main(int argc, char** argv)
{
		// parsed image, and filtered image
		rgb *image, *filteredImage;

		// image attributes
		int global_width, global_height;
		int width, height, max;
		dimension *my_dim;

		double start_time = 0.0, run_time;

		// process information
		int my_rank, p;

		// loop variables
		int dest, source;
		int offset;

		// MPI boilerplate
		MPI_Status status;
		MPI_Init(&argc, &argv);
		MPI_Comm_size(MPI_COMM_WORLD, &p);
		MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

		char processor_name[MPI_MAX_PROCESSOR_NAME];
		int name_len;
		MPI_Get_processor_name(processor_name, &name_len);

		char rank_msg[5];
		char process_msg[5];

		sprintf(rank_msg, "%d", my_rank);
		sprintf(process_msg, "%d\n", p);

		printc("Starting process from ", 2);
		printc(processor_name, 6);
		printc(" with rank ", 2);
		printc(rank_msg, 6);
		printc(" of ", 2);
		printc(process_msg, 6);

		int tag = 0;
		int window_size = 5;

		char ppm_convert_in[100];
		char ppm_convert_out[100];

		// Process 0 parses the PPM file and distributes size attributes
		if (my_rank == 0)
		{
				sprintf(ppm_convert_in, "convert %s tmp_in.ppm", argv[1]);

				system(ppm_convert_in);
				system("convert tmp_in.ppm -compress none tmp_in.ppm");

				image = read_ppm("tmp_in.ppm", &global_width, &global_height, &max);

				start_time = MPI_Wtime();

				offset = 0;

				for (dest = 1; dest < p; dest++)
				{
						int rowsToCompute = global_height/p;
						if (dest < global_height % p)
								rowsToCompute++;
						offset += rowsToCompute;

						// Need window_size/2 (or 2*window_size/2) slack to make sure that the borders are computed correctly
						if (dest == p - 1)
						{
								rowsToCompute += (window_size/2);
						}
						else
						{
								rowsToCompute += (window_size/2)*2;
						}

						dimension *dim = (dimension*)malloc(sizeof(dimension));
						dim->width = global_width;
						dim->height = rowsToCompute;
						MPI_Send(dim, 2, MPI_INT, dest, tag, MPI_COMM_WORLD);
				}

				// Set own size attributes
				width = global_width;
				height = global_height / p;
				if (global_height % p != 0)
				{
						height += 1;
				}
				height += (window_size / 2);
		}

		// Receive size attributes
		else
		{
				my_dim = (dimension*)malloc(sizeof(dimension));
				MPI_Recv(my_dim, 2, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
				width = my_dim->width;
				height = my_dim->height;
				image = (rgb*)malloc(height*width*sizeof(rgb));
		}


		// Process 0 distributes rows
		if (my_rank == 0)
		{
				offset = global_height / p;
				if (global_height % p != 0)
				{
						offset += 1;
				}
				offset += (window_size / 2);
				for (dest = 1; dest < p; dest++)
				{
						int rowsToCompute = global_height / p;
						if (dest < global_height % p)
								rowsToCompute++;

						if (dest == p - 1)
						{
								rowsToCompute += (window_size / 2);
						}
						else
						{
								rowsToCompute += (window_size / 2) * 2;
						}

						if (dest == p - 1)
						{
								offset -= (window_size / 2) * 2;
						}
						else
						{
								offset -= (window_size / 2) * 2;
						}


						MPI_Send(image + offset * width, 3 * rowsToCompute * width, MPI_UNSIGNED_CHAR, dest, tag, MPI_COMM_WORLD);
						offset += rowsToCompute;
				}
		}

		// Receive rows
		else
		{
				MPI_Recv(image, height * width * 3, MPI_UNSIGNED_CHAR, 0, tag, MPI_COMM_WORLD, &status);
		}


		// Process image	
		filteredImage = process_image(width, height, image, window_size);


		// Send processed data back to P0
		if (my_rank != 0 && my_rank != p - 1)
		{
				MPI_Send(image + (window_size / 2) * width, (height - 2 * (window_size / 2)) * width * 3, MPI_UNSIGNED_CHAR, 0, tag, MPI_COMM_WORLD);
		}
		else if (my_rank == p - 1)
		{
				MPI_Send(image + (window_size / 2) * width, (height - (window_size / 2)) * width * 3, MPI_UNSIGNED_CHAR, 0, tag, MPI_COMM_WORLD);
		}
		else
		{
				offset = global_height / p;
				if (global_height % p != 0)
				{
						offset += 1;
				}
				for (source = 1; source < p; source++)
				{
						int rowsToCompute = global_height/p;
						if (source < global_height % p)
								rowsToCompute++;

						MPI_Recv(image+offset*width, rowsToCompute*width*3, MPI_UNSIGNED_CHAR, source, tag, MPI_COMM_WORLD, &status);

						offset += rowsToCompute;
				}
		}

		// Process 0 writes processed PPM file
		if (my_rank == 0)
		{
				run_time = MPI_Wtime() - start_time;

				write_ppm("tmp_out.ppm", global_width, global_height, max, filteredImage);
				free(image);

				sprintf(ppm_convert_out, "convert tmp_out.ppm %s", argv[2]);
				system(ppm_convert_out);

				system("rm tmp_*");

				char time_msg[20];
				sprintf(time_msg, "\t\t\t %f\n", run_time);

				printc("################ METRICS #################\n", 6);
				printc("TIME:", 2);
				printc(time_msg, 3);
		}

		MPI_Finalize();

		return(0);
}