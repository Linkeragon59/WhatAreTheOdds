class WhatAreTheOddsAPI {

  constructor(serviceAddress) {
    this.serviceAddress = serviceAddress;
  }

  calculateOdds(file, callback) {
    fetch(this.serviceAddress, {
      method: 'POST',
      headers: {
        "Content-Type": "application/json"
      },
      body: file
    }).then(
      res => res.json()
    ).then(
      (response)=> {
        console.log(response);
        callback(response["odds"]);
      },
      (err)=> {
        console.log(err);
      }
    );
  }
}

export default WhatAreTheOddsAPI;
