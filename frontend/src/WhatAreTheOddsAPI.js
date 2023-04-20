class WhatAreTheOddsAPI {

  constructor(serviceAddress) {
    this.serviceAddress = serviceAddress;
  }

  calculateOdds(file, successCallback, errorCallback) {
    fetch(this.serviceAddress, {
      method: 'POST',
      headers: {
        "Content-Type": "application/json"
      },
      body: file
    }).then(response => {
      if (response.ok) {
        response.json().then(jsonResponse => {
          successCallback(Math.round(jsonResponse["odds"] * 100) + "%");
        })
      } else {
        response.text().then(textResponse => {
          errorCallback(textResponse);
        })
      }
    }).catch(error => {
      console.error(error);
      errorCallback("The file couldn't be uploaded");
    });
  }
}

export default WhatAreTheOddsAPI;
