/*
Based on code by https://github.com/PhilGabardo/PPM-Filtering
*/

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
		rgb *noisy_image, *filtered_image;

		// image attributes
		int total_width, total_height;
		int width, height, max;
		dimension *dim;

		double start_time = 0.0, run_time;
		int rank, total_process;
		int offset;

		/* Init MPI library */
		MPI_Status status;
		MPI_Init(&argc, &argv);
		MPI_Comm_size(MPI_COMM_WORLD, &total_process);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);

		char processor_name[MPI_MAX_PROCESSOR_NAME];
		int name_len;
		MPI_Get_processor_name(processor_name, &name_len);

		char rank_msg[5];
		char process_msg[5];

		sprintf(rank_msg, "%d", rank);
		sprintf(process_msg, "%d\n", total_process);

		/* Prints initialize process */
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

		/* read image and sends images part info */
		if (rank == 0)
		{
				sprintf(ppm_convert_in, "convert %s tmp_in.ppm", argv[1]);

				system(ppm_convert_in);
				system("convert tmp_in.ppm -compress none tmp_in.ppm");

				noisy_image = read_ppm("tmp_in.ppm", &total_width, &total_height, &max);

				start_time = MPI_Wtime();

				offset = 0;

				for (int dest = 1; dest < total_process; dest++)
				{
						int rows = total_height / total_process;

						if (dest < total_height % total_process)
								rows++;

						offset += rows;

						if (dest == total_process - 1)
						{
								rows += (window_size / 2);
						}
						else
						{
								rows += (window_size / 2) * 2;
						}

						dimension *curr_dim = (dimension *)malloc(sizeof(dimension));
						curr_dim->width = total_width;
						curr_dim->height = rows;
						MPI_Send(curr_dim, 2, MPI_INT, dest, tag, MPI_COMM_WORLD);
				}

				/* Set attributes */
				width = total_width;
				height = total_height / total_process;
				if (total_height % total_process != 0)
				{
						height += 1;
				}
				height += (window_size / 2);
		}

		/* Receive image size */
		else
		{
				dim = (dimension *)malloc(sizeof(dimension));
				MPI_Recv(dim, 2, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
				width = dim->width;
				height = dim->height;
				noisy_image = (rgb *)malloc(height * width * sizeof(rgb));
		}


		/* Sends image data */
		if (rank == 0)
		{
				offset = total_height / total_process;

				if (total_height % total_process != 0)
				{
						offset += 1;
				}

				offset += (window_size / 2);

				for (int dest = 1; dest < total_process; dest++)
				{
						int rows = total_height / total_process;
						if (dest < total_height % total_process)
								rows++;

						if (dest == total_process - 1)
						{
								rows += (window_size / 2);
						}
						else
						{
								rows += (window_size / 2) * 2;
						}

						if (dest == total_process - 1)
						{
								offset -= (window_size / 2) * 2;
						}
						else
						{
								offset -= (window_size / 2) * 2;
						}

						MPI_Send(noisy_image + offset * width, 3 * rows * width, MPI_UNSIGNED_CHAR, dest, tag, MPI_COMM_WORLD);
						offset += rows;
				}
		}
		/* Receive image data */
		else
		{
				MPI_Recv(noisy_image, height * width * 3, MPI_UNSIGNED_CHAR, 0, tag, MPI_COMM_WORLD, &status);
		}

		/* Process image */
		filtered_image = process_image(width, height, noisy_image, window_size);

		/* Send back data to process 0 */
		/* Not first and not last process */
		if (rank != 0 && rank != total_process - 1)
		{
				MPI_Send(noisy_image + (window_size / 2) * width, (height - 2 * (window_size / 2)) * width * 3, MPI_UNSIGNED_CHAR, 0, tag, MPI_COMM_WORLD);
		}
		/* last process */
		else if (rank == total_process - 1)
		{
				MPI_Send(noisy_image + (window_size / 2) * width, (height - (window_size / 2)) * width * 3, MPI_UNSIGNED_CHAR, 0, tag, MPI_COMM_WORLD);
		}
		/* process 0 */
		else
		{
				offset = total_height / total_process;

				if (total_height % total_process != 0)
				{
						offset += 1;
				}
				for (int source = 1; source < total_process; source++)
				{
						int rows = total_height / total_process;

						if (source < total_height % total_process)
								rows++;

						MPI_Recv(noisy_image + offset * width, rows * width * 3, MPI_UNSIGNED_CHAR, source, tag, MPI_COMM_WORLD, &status);

						offset += rows;
				}
		}

		/* Write filtered image, for process 0 */
		if (rank == 0)
		{
				run_time = MPI_Wtime() - start_time;

				write_ppm("tmp_out.ppm", total_width, total_height, max, filtered_image);
				free(noisy_image);

				sprintf(ppm_convert_out, "convert tmp_out.ppm %s", argv[2]);
				system(ppm_convert_out);

				system("rm tmp_*");

				char time_msg[20];
				sprintf(time_msg, "\t\t\t %f\n", run_time);

				printc("################ METRICS #################\n", 6);
				printc("TIME:", 2);
				printc(time_msg, 3);
		}

		/* Finalize MPI */
		MPI_Finalize();

		return(0);
}