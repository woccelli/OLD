package com.c1p.velov;

import com.c1p.velov.model.Prediction;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.autoconfigure.EnableAutoConfiguration;
import org.springframework.format.annotation.DateTimeFormat;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import java.time.LocalDateTime;
import java.util.Date;
import java.util.List;

@CrossOrigin(origins = "*")
@RestController
@EnableAutoConfiguration
public class Endpoints {

  private final TrainedModelExecutor trainedModelExecutor;

  @Autowired
  public Endpoints(TrainedModelExecutor trainedModelExecutor) {
    this.trainedModelExecutor = trainedModelExecutor;
  }

  @GetMapping("/predict/time")
  public List<Prediction> predictForDate(
    @DateTimeFormat(pattern = "yyyy-MM-dd@HH:mm:ss") @RequestParam LocalDateTime dateTime) {
    return trainedModelExecutor.predictForAllStations(dateTime);
  }

  @GetMapping("/predict/station")
  public List<Prediction> predictForStation(
    @RequestParam long stationId,
    @DateTimeFormat(pattern = "yyyy-MM-dd@HH:mm:ss") @RequestParam LocalDateTime dateTime) {
    return trainedModelExecutor.predictForAllTimes(dateTime, stationId);
  }

  @GetMapping("/predict/all")
  public List<Prediction> predictAll(
    @DateTimeFormat(pattern = "yyyy-MM-dd@HH:mm:ss") @RequestParam LocalDateTime date) {
    return trainedModelExecutor.predictForAllStationsAllTimes(date);
  }

}
