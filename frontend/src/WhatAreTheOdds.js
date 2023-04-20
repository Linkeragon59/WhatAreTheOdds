import React from 'react'

function WhatAreTheOddsForm(props) {

  return(
    <div>
      <div>
        <label>Upload the Empire data</label>
      </div>
      <div>
        <input type="file" accept="application/JSON"/>
        <input type="button" value="Submit" onClick={props.onSubmitClicked}/>
      </div>
      <div>
        <label>{props.odds}</label>
      </div>
    </div>
  );

}

class WhatAreTheOdds extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      odds: "Success odds: Unknown",
    };
  }
  
  onSubmitClicked() {
    var input = document.querySelector('input[type="file"]')
    this.props.WhatAreTheOddsAPI.calculateOdds(input.files[0],
    (oddsResult)=> {
      this.setState({
        odds: "Success odds: " + oddsResult,
      });
    },
    (errorResult) => {
      this.setState({
        odds: errorResult,
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
