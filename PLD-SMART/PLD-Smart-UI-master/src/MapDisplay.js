import React from 'react'
import { Map as LeafletMap, TileLayer, Marker, Popup } from 'react-leaflet';
import L from 'leaflet';
import Icon from './SVGIconComponent';
import ReactDOMServer from 'react-dom/server';


class Map extends React.Component {
  constructor(props){
    super(props);
    this.state = {
      stations: props.stations
    };
  }

  onClickStation = (station) => {
    this.props.onClickStation(station)
  };

  render() {
    return (
      <LeafletMap
        className='map'
        center={[ 45.764043,  4.835659]}
        zoom={12}
        maxZoom={20}
        attributionControl={true}
        zoomControl={true}
        doubleClickZoom={true}
        scrollWheelZoom={true}
        dragging={true}
        animate={true}
        easeLinearity={0.35}
      >
        <TileLayer
          //url='http://{s}.tile.osm.org/{z}/{x}/{y}.png'
          url='https://{s}.basemaps.cartocdn.com/rastertiles/voyager/{z}/{x}/{y}{r}.png'

          //url='https://api.tiles.mapbox.com/v4/mapbox.light/{z}/{x}/{y}.png?access_token=pk.eyJ1IjoibWFwYm94IiwiYSI6ImNpejY4NXVycTA2emYycXBndHRqcmZ3N3gifQ.rJcFIG214AriISLbB6B5aw'
        />
        {this.props.stations ?
          this.props.stations.map((station) => (
          <Marker onClick = {() => this.onClickStation(station)}
                  position={[ station.position.lat, station.position.lng]}
                  icon={L.divIcon({
                    className: 'custom-icon',
                    html: ReactDOMServer.renderToString(<Icon nbBikes={station.available_bikes} nbStands={station.bike_stands}/>)
                  })}>
          </Marker>)
        ) : <div/>}

      </LeafletMap>
    );
  }
}

export default Map