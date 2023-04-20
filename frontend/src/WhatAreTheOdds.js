import React from 'react'

function WhatAreTheOddsForm(props) {

  return(
    <div>
      <label>Upload the Empire data</label>
      <div>
        <input type="file"/>
        <input type="button" value="Submit" onClick={props.onSubmitClicked}/>
      </div>
      <label>Success odds: {props.odds}</label>
    </div>
  );

}

class WhatAreTheOdds extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      odds: "Unknown",
    };
  }
  
  onSubmitClicked() {
    var input = document.querySelector('input[type="file"]')
    this.props.WhatAreTheOddsAPI.calculateOdds(input.files[0], (result)=> {
      var oddsPercentage = Math.round(result * 100) + "%";
      this.setState({
        odds: oddsPercentage,
      });
    });
  }
  
  render() {
    return (
      <div>
        <WhatAreTheOddsForm
          odds={this.state.odds}
          onSubmitClicked={this.onSubmitClicked.bind(this)}
        />
      </div>
    );
  }
}

export default WhatAreTheOdds;
