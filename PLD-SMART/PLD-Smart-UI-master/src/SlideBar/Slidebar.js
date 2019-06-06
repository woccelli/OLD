import React from 'react'
import Histogram from './Histogram.js';
import '../App.css';




class Slidebar extends React.Component {
    constructor(props){
        super(props);
    }

  render() {
    return (
      <div>
        <Histogram
          data = {this.props.data}/>
      </div>
    );
  }
}

export default Slidebar;