package com.c1p.velov;

import com.c1p.velov.model.Prediction;
import org.deeplearning4j.nn.modelimport.keras.exceptions.InvalidKerasConfigurationException;
import org.deeplearning4j.nn.modelimport.keras.exceptions.UnsupportedKerasConfigurationException;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.time.LocalDateTime;
import java.util.Date;
import java.util.List;

import static org.junit.Assert.*;

public class TrainedModelExecutorTest {

  private final int NB_STATIONS = 332;

  private TrainedModelExecutor trainedModelExecutor;

  @Before
  public void setUp() throws UnsupportedKerasConfigurationException, IOException, InvalidKerasConfigurationException {
    trainedModelExecutor = new TrainedModelExecutor();
  }

  @Test
  public void trainedModelLoads() {
    assertNotNull(trainedModelExecutor);
  }

  @Test
  public void modelComputesPredictionForAllStations() {
    // given

    // when
    List<Prediction> result = trainedModelExecutor.predictForAllStations(LocalDateTime.now());

    // then
    assertNotNull(result);
    assertEquals(NB_STATIONS, result.size());
  }

  @Test
  public void modelComputesPredictionForAllTimes() {
    // given
    LocalDateTime now = LocalDateTime.now();
    int stationId = 0;

    // when
    List<Prediction> predictions = trainedModelExecutor.predictForAllTimes(now, stationId);

    // then
    assertEquals(24, predictions.size());
  }

  @Test
  public void modelComputesPredictionForAllStationsAllTimes() {
    // given
    LocalDateTime now = LocalDateTime.now();

    // when
    List<Prediction> predictions = trainedModelExecutor.predictForAllStationsAllTimes(now);

    // then
    assertEquals(24*NB_STATIONS, predictions.size());
  }


}