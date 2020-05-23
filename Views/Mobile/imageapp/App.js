import React, { Component } from 'react';

import { StyleSheet, Text, View, PixelRatio, ToastAndroid, TouchableOpacity, Image, TextInput, Alert } from 'react-native';

import ImagePicker from 'react-native-image-picker';


var Buffer = require('buffer/').Buffer  // note: the trailing slash is important!

var fs = require('react-native-fs');

console.disableYellowBox = true;

export default class Project extends Component {

  constructor() {

    super();
    
    this.state = {

      ImageSource: null,
      data: null,
      ip:'',
      port:'',

    }
  }

  selectPhotoTapped() {
    const options = {
      quality: 1.0,
      maxWidth: 500,
      maxHeight: 500,
      storageOptions: {
        skipBackup: true
      }
    };

    ImagePicker.showImagePicker(options, (response) => {
      console.log('Response = ', response);

      if (response.didCancel) {
        console.log('User cancelled photo picker');
      }
      else if (response.error) {
        console.log('ImagePicker Error: ', response.error);
      }
      else if (response.customButton) {
        console.log('User tapped custom button: ', response.customButton);
      }
      else {
        let source = {uri:response.uri };

        this.setState({
          path: response.uri,
          ImageSource: source,
          data: response.data,



        });
      }
    });
  }

  uploadImageToServer = async () => {

      const base64 = await fs.readFile(this.state.path, 'base64')
      const buffer = Buffer.from(base64, 'base64')
      return fetch(`http://${this.state.ip}:${this.state.port}`, {
        method: 'PUT',
        headers: {
        'Content-Type': 'image/png; charset=utf-8',
        'x-amz-acl': 'public-read',
      },
        body: buffer,
      })
      .then((res) => { console.log("image success uploaded") })
      .catch(function(error) {
        console.log('There has been a problem with your fetch operation: ' + error.message);
         // ADD THIS THROW error
          throw error;
      });

      this.props.writeText(this.state.text)

      this.setState({
        text: ''
      })
  }

  initImageToServer = async () => {
    ToastAndroid.show(`http://${this.state.ip}:${this.state.port}`, ToastAndroid.SHORT);
    const msg = 'init';
    return fetch(`http://${this.state.ip}:${this.state.port}`, {
      method: 'PUT',
      body: msg,
    })
    .then((res) => { console.log("image success uploaded") })
    .catch(function(error) {
      console.log('There has been a problem with your fetch operation: ' + error.message);
       // ADD THIS THROW error
        throw error;
    });

    this.props.writeText(this.state.text)

    this.setState({
      text: ''
    })
}


  render() {
    return (
    
      <View style={styles.container}>
        <Text style={styles.Title}>Upload image to Server</Text>

        <TouchableOpacity onPress={this.selectPhotoTapped.bind(this)}>

          <View style={styles.ImageContainer}>

            {this.state.ImageSource === null ? <Text>CHOOSE IMAGE</Text> :
              <Image style={styles.ImageContainer} source={this.state.ImageSource} path = {this.state.ImageSource} />
            }

          </View>
          

        </TouchableOpacity>


        <TextInput
          keyboardType='numeric'
            
          placeholder="IP ..."

          onChangeText={ip => this.setState({ ip })}
          
          value = {this.state.ip} 

          underlineColorAndroid='transparent'

          style={styles.TextInputStyle}
        />

        <TextInput
          keyboardType='numeric'

          placeholder="Port ..."

          onChangeText={port => this.setState({ port })}

          value = {this.state.port} 

          underlineColorAndroid='transparent'

          style={styles.TextInputStyle}
        />


        <TouchableOpacity onPress={this.uploadImageToServer} activeOpacity={0.6} style={styles.button} >

          <Text style={styles.TextStyle}> SEND IMAGE </Text>

        </TouchableOpacity>

        <TouchableOpacity onPress={this.initImageToServer} activeOpacity={0.6} style={styles.button} >

          <Text style={styles.TextStyle}> INIT PROCESS </Text>

        </TouchableOpacity>


      </View>
    );
  }

}

const styles = StyleSheet.create({

  container: {
    flex: 1,
    alignItems: 'center',
    backgroundColor: '#FFFFFF',
    paddingTop: 20
  },

  ImageContainer: {
    borderRadius: 10,
    width: 250,
    height: 250,
    borderColor: '#dddddd',
    borderWidth: 1 / PixelRatio.get(),
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#dddddd',

  },

  TextInputStyle: {

    textAlign: 'center',
    height: 40,
    width: '80%',
    borderRadius: 10,
    borderWidth: 1,
    borderColor: '#dddddd',
    marginTop: 20
  },

  button: {

    width: '80%',
    backgroundColor: '#00b28f',
    borderColor: '#dddddd',
    borderRadius: 7,
    marginTop: 20
  },

  TextStyle: {
    color: '#fff',
    textAlign: 'center',
    padding: 10
  },

  Title:{
    color: '#000000',
    textAlign: 'center',
    padding: 20,
    fontSize: 32,
    fontWeight: 'bold'
  }

});