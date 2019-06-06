import React from 'react';
import './App.css';

import Paper from '@material-ui/core/Paper';
import Typography from '@material-ui/core/Typography';

export default class StationInformation extends React.Component {
  constructor(props){
    super(props);
  }

  render(){
    return(
      this.props.currentStation ?
        <div className="station details">
        <hr/>
          <h3>
            {(this.props.currentStation.name).substring((this.props.currentStation.name).indexOf('-')+2 )}
          </h3>
          
          <div className="frame_btn">

              <div className="left_velov_btn">
              <Paper elevation={1}>
                <Typography variant="h5" component="h3">
                {this.props.currentStation.available_bikes}
                </Typography>
                <Typography component="p">
                  vélos disponibles                
                </Typography>
              </Paper>
              </div>

              <div className="right_velov_btn">
              <Paper elevation={1}>
                <Typography variant="h5" component="h3">
                  {this.props.currentStation.available_bike_stands}                
                </Typography>
                <Typography component="p">
                  places disponibles               
                </Typography>
              </Paper>
              </div>
            </div>
            <br/>
            <hr/>
          </div>
        :
      <h3>
        <hr></hr>
        Choisissez une station
      </h3>
    )
  }
}