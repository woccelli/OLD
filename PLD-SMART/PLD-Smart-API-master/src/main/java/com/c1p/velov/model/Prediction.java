package com.c1p.velov.model;

import com.fasterxml.jackson.annotation.JsonFormat;

import java.time.LocalDateTime;
import java.util.Date;
import java.util.Map;

/**
 * Represents a bike / bike stand availability for a single station, at a given date/time
 */
public class Prediction {

  private long stationId;
  private int nbAvailableBikes;
  private int nbAvailableStands;
  private LocalDateTime dateTime;

  public Prediction(long stationId, int nbAvailableBikes, int nbAvailableStands, LocalDateTime dateTime) {
    this.stationId = stationId;
    this.nbAvailableBikes = nbAvailableBikes;
    this.nbAvailableStands = nbAvailableStands;
    this.dateTime = dateTime;
  }

  public long getStationId() {
    return stationId;
  }

  public void setStationId(long stationId) {
    this.stationId = stationId;
  }

  public int getNbAvailableBikes() {
    return nbAvailableBikes;
  }

  public void setNbAvailableBikes(int nbAvailableBikes) {
    this.nbAvailableBikes = nbAvailableBikes;
  }

  public int getNbAvailableStands() {
    return nbAvailableStands;
  }

  public void setNbAvailableStands(int nbAvailableStands) {
    this.nbAvailableStands = nbAvailableStands;
  }

  public LocalDateTime getDateTime() {
    return dateTime;
  }

  public void setDateTime(LocalDateTime dateTime) {
    this.dateTime = dateTime;
  }
}
