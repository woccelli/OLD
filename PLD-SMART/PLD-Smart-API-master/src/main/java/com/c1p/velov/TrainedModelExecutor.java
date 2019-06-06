package com.c1p.velov;

import com.c1p.velov.model.Prediction;
import org.deeplearning4j.nn.modelimport.keras.KerasModelImport;
import org.deeplearning4j.nn.modelimport.keras.exceptions.InvalidKerasConfigurationException;
import org.deeplearning4j.nn.modelimport.keras.exceptions.UnsupportedKerasConfigurationException;
import org.deeplearning4j.nn.multilayer.MultiLayerNetwork;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.factory.Nd4j;
import org.springframework.core.io.ClassPathResource;
import org.springframework.stereotype.Component;

import java.io.IOException;
import java.time.LocalDateTime;
import java.util.*;

@Component
class TrainedModelExecutor {

  // these three maps need to be computed from the data, and passed to the back-end
  private final Map<Integer, Long> indexToId;
  private final Map<Long, Integer> idToIndex;
  private final Map<Long, Integer> idToCapacity;
  private static final int OUTPUT_SIZE = 332;

  // path to trained model generated by tensorflow, with .proto and .sd files
  private static final String REL_PATH_TO_MODEL = "model/full_model.h5";
  private MultiLayerNetwork network;

  TrainedModelExecutor() throws IOException, InvalidKerasConfigurationException, UnsupportedKerasConfigurationException {
    // temporarily fill these maps with BS until right data is passed
    indexToId = new HashMap<>();
    idToIndex = new HashMap<>();
    idToCapacity = new HashMap<>();
    for (int i = 0; i < OUTPUT_SIZE; i++) {
      indexToId.put(i, (long) i);
      idToIndex.put((long) i, i);
      idToCapacity.put((long) i, 20);
    }
    // Load neural network
    String absPathToModel = new ClassPathResource(REL_PATH_TO_MODEL).getFile().getPath();
    network = KerasModelImport.importKerasSequentialModelAndWeights(absPathToModel);
  }


  /**
   * Makes an availability prediction for all bike stations,
   * at a specific time
   *
   * @param date the date and time of the prediction
   * @return the list of predictions
   */
  List<Prediction> predictForAllStations(LocalDateTime date) {

    // Set date to start of param minute
    date = date
      .withNano(0)
      .withSecond(0);

    // create an input Tensor
    double[] params = {0f, 2f};
    INDArray input = Nd4j.create(params);

    // run the model and get the results for this hour
    INDArray output = network.output(input);
    double[] results = output.toDoubleVector();

    return buildPredictions(date, results);
  }




  /**
   * Makes an availability prediction for a specific  bike station,
   * over the course of a whole day
   * One prediction every hour: [midnight, midnight[
   *
   * @param date the day of the prediction
   * @return the list of predictions
   */
  List<Prediction> predictForAllTimes(LocalDateTime date, long stationId) {

    // get position of target station within brute results
    int stationIndex = idToIndex.get(stationId);
    ArrayList<Prediction> predictions = new ArrayList<>();

    // Set date to start of param day
    date = date
      .withNano(0)
      .withSecond(0)
      .withMinute(0);


    // iterate on all hours
    for (int hour = 0; hour < 24; hour++) {
      date = date.withHour(hour);
      double[] params = {0f, (float) hour};
      INDArray input = Nd4j.create(params);

      // run the model and get the results for this hour
      INDArray output = network.output(input);
      double[] results = output.toDoubleVector();

      int nbBikes = (int) results[stationIndex];
      int totalStands = idToCapacity.get(stationId);
      int nbFreeStands = totalStands - nbBikes;

      Prediction prediction = new Prediction(stationId, nbBikes, nbFreeStands, date);
      predictions.add(prediction);
    }
    return predictions;
  }


  /**
   * Makes an availability prediction for all bike stations,
   * over the course of a whole day
   * One prediction every hour: [midnight, midnight[
   *
   * @param date the day of the prediction
   * @return the list of predictions
   */
  List<Prediction> predictForAllStationsAllTimes(LocalDateTime date) {
    ArrayList<Prediction> predictions = new ArrayList<>();

    // Set date to start of param day
    date = date
      .withNano(0)
      .withSecond(0)
      .withMinute(0);
    // iterate on all hours
    for (int hour = 0; hour < 24; hour++) {
      double[] params = {0f, (float) hour};
      INDArray input = Nd4j.create(params);

      // run the model and get the results for this hour
      INDArray output = network.output(input);
      double[] results = output.toDoubleVector();

      predictions.addAll(buildPredictions(date, results));
    }
    return predictions;
  }


  /**
   * Build an array of prediction objects from brute results
   * @param date day and time of the prediction was made for
   * @param results brute prediction results from Neural Network
   * @return the built prediction objects
   */
  private ArrayList<Prediction> buildPredictions(LocalDateTime date, double[] results) {
    ArrayList<Prediction> predictions = new ArrayList<>();
    for (int i = 0; i < results.length; i++) {
      int nbBikes = (int) Math.round(results[i]);
      long stationId = indexToId.get(i);
      int totalStands = idToCapacity.get(stationId);
      int nbFreeStands = totalStands - nbBikes;

      Prediction prediction = new Prediction(stationId, nbBikes, nbFreeStands, date);
      predictions.add(prediction);
    }
    return predictions;
  }


}
