import 'date-fns';
import React from 'react';
import Grid from '@material-ui/core/Grid';
import Fab from '@material-ui/core/Fab';
import NavigationIcon from '@material-ui/icons/Navigation';
import {withStyles} from '@material-ui/core/styles';
import DateFnsUtils from '@date-io/date-fns';
import {MuiPickersUtilsProvider, TimePicker, DatePicker} from 'material-ui-pickers';
import Slider from '@material-ui/lab/Slider';

const styles = {
  grid: {
    width: '90%',
  },
  sliderColor: {
    color: 'red',
  }
};

class MaterialUIPickers extends React.Component {
  state = {
    // The first commit of Material-UI
    selectedDate: new Date(),
    value: 0,
    showSlider: false,
    selectedTime: new Date(2019, 5, 8, 0, 0, 0, 0)
  };

  handleDateChange = date => {
    this.setState({selectedDate: date});
  };

  handleChange = (event, value) => {
    let newTime = new Date(2019, 5, 8, 0, 0, 0, 0);
    newTime.setMinutes(this.state.selectedTime.getMinutes() + 30 * value);
    this.setState({value: value, selectedTime: newTime});
    this.props.handleHourChange(value);
  };


  render() {
    const {classes} = this.props;
    const {selectedDate} = this.state;

    return (
      <MuiPickersUtilsProvider utils={DateFnsUtils}>
        {/*<Grid container className={classes.grid} justify="space-around">*/}
        <DatePicker
          margin="normal"
          label="Date de prévision"
          value={selectedDate}
          onChange={this.handleDateChange}
        />
        <br/>
        <Fab variant="extended" aria-label="Delete" className={classes.fab}
             onClick={() => {
               this.props.handlePredictByDate(this.state.selectedDate);
               this.setState({showSlider: true});
             }}>
          <NavigationIcon/>
          Calculer ma prévision !
        </Fab>
        <br/>
        {this.state.showSlider ?
          <div style={{'margin-left': '5%', 'margin-right': '5%'}}>
            <br/>
            <Slider
              classes={{container: classes.slider}}
              value={this.state.value}
              min={0}
              max={47}
              step={1}
              aria-labelledby="label"
              onChange={this.handleChange}
            />
            <div>
              {this.state.selectedTime.getHours().toString().padStart(2, '0') + ':' + this.state.selectedTime.getMinutes().toString().padStart(2, '0')}
            </div>
          </div>
          :
          <div/>
        }
        {/*</Grid>*/}
      </MuiPickersUtilsProvider>
    );
  }
}


export default withStyles(styles)(MaterialUIPickers);