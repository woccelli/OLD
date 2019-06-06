import React from 'react';
import './App.css';
import Slidebar from './SlideBar/Slidebar.js'
import MapDisplay from './MapDisplay.js';
import StationInformation from './StationInformation.js';
import {getStations, predict} from "./Api.js";
import MaterialUIPickers from './CalendarPicker.js';
import {toast, ToastContainer} from "react-toastify";
import 'react-toastify/dist/ReactToastify.css';
import logo from './logo.png';

class App extends React.Component {
  constructor(props) {
    super(props);
    this.handleStationRequest();
    this.state = {
      stations: null,
      currentStation: null,
      showHistogram: false
    };
  };

  handleStationRequest = () => {
    getStations().then((stations) => {
      this.setState({
        showHistogram: false,
        stations: stations,
      });
      this.state.stations.forEach((e) => e.evolution = [])
    }).catch(() => {
      toast.error("Could not reach server");
      this.setState({
          stations: null
        }
      )
    })
  };


  handlePredict = (date) => {
    // put prediction in the then argument
  predict(date).then((prediction) => {
    //   console.log(prediction);
      let updatedStation;
      let tempEvolution = [];
      let tempStations = [];
      let time = new Date();
      time.setHours(0);
      time.setMinutes(0);
      for (const [id, evolution] of Object.entries(prediction)) {
        updatedStation = this.state.stations.find((s) => {
          return s.number === parseInt(id);
        });
        if (updatedStation) {
          evolution.forEach((h) => {
            tempEvolution.push(
              {
                name: time.getHours().toString().padStart(2, '0') + ':' + time.getMinutes().toString().padStart(2, '0'),
                Disponibles: h,
                Occupes: Math.max(updatedStation.bike_stands - h,0)
              }
            );
            time.setMinutes(time.getMinutes() + 30);
          });
          updatedStation.evolution = tempEvolution;
          updatedStation.available_bikes = tempEvolution[0].Disponibles;
          updatedStation.available_bike_stands = tempEvolution[0].Occupes;
          tempEvolution = [];
          tempStations.push(updatedStation);
        }
      }
      this.setState({
        stations: tempStations
      });
   });
  };

  handleHourChange = (hour) => {
    let currentStations = this.state.stations;
    currentStations.forEach((s) => {
      s.available_bikes = s.evolution[hour].Disponibles;
      s.available_bike_stands = s.evolution[hour].Occupes;
    })
    this.setState({stations: currentStations})
  };

  onClickStation = (station) => {
    this.setState({
        currentStation: station,
        showHistogram: true
      }
    );
  };


  render() {
    return (
      <div className="App">
        <ToastContainer/>
        <div className="split left">
          {this.state.stations ?
            <MapDisplay
              stations={this.state.stations}
              onClickStation={this.onClickStation}/>
            :
            <MapDisplay/>}
          <div className="histogram">
            {this.state.showHistogram ?
              <Slidebar
                data={this.state.currentStation.evolution}
              />
              : <div/>}

          </div>
        </div>

        <div className="split right">
          <img src={logo} style={{'height':'40%'}}/>
          <StationInformation
            currentStation={this.state.currentStation}/>
          <MaterialUIPickers
            handlePredictByDate={this.handlePredict}
            handleHourChange={this.handleHourChange}/>
        </div>
      </div>
    );
  }
}

export default App;
