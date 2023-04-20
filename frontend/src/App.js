import './App.css';
import WhatAreTheOdds from './WhatAreTheOdds'
import WhatAreTheOddsAPI from './WhatAreTheOddsAPI'

function App() {
  return (
    <div className="App">
      <header className="App-header">
        <WhatAreTheOdds WhatAreTheOddsAPI={new WhatAreTheOddsAPI('http://127.0.0.1:3001/odds')}/>
      </header>
    </div>
  );
}

export default App;
