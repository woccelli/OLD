import React, { PureComponent } from 'react';
import {
  BarChart, Bar, Cell, XAxis, YAxis, CartesianGrid, Tooltip, Legend,
} from 'recharts';

export default class Histogram extends React.Component {

  render() {
    return (
      <BarChart
        width={1000}
        height={200}
        data={this.props.data}
        margin={{
          top: 5, right: 30, left: 0, bottom: 5,
        }}
      >
        <CartesianGrid strokeDasharray="3 3" />
        <XAxis dataKey="name" />
        <YAxis />
        <Tooltip />
        <Legend />
        <Bar dataKey="Disponibles" fill="#ff0000" />
        <Bar dataKey="Occupes" fill="#82zca9d" />
      </BarChart>
    );
  }
}
