#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

typedef struct training_data {
  float x1;
  float x2;
  float y;
}training_data_t;

typedef struct test_data {
  float x1;
  float x2;
}test_data_t;

#define INPUT_DIMENTION   2
#define N_NEURON          3

typedef struct model {
  float weights[N_NEURON][INPUT_DIMENTION];
  float biases[N_NEURON];
}model_t;

int n_test = 4;
int n_train = 4;
int n_epoch = 100000000;
float acceptable_error = 1e-3;
float eps = 1e-2;
float learning_rate = 1e-1;

training_data_t training_data_or_gate[] = {
  {0, 0, 0},
  {0, 1, 1},
  {1, 0, 1},
  {1, 1, 1}
};

training_data_t training_data_and_gate[] = {
  {0, 0, 0},
  {0, 1, 0},
  {1, 0, 0},
  {1, 1, 1}
};

training_data_t training_data_nor_gate[] = {
  {0, 0, 1},
  {0, 1, 0},
  {1, 0, 0},
  {1, 1, 0}
};

training_data_t training_data_xor_gate[] = {
  {0, 0, 0},
  {0, 1, 1},
  {1, 0, 1},
  {1, 1, 0}
};

test_data_t test_data_common[] = {
  {0, 0},
  {0, 1},
  {1, 0},
  {1, 1}
};

void model_init(model_t *);
void train(model_t *, training_data_t *);
void test(model_t *, test_data_t *);
float sigmoid(float);
float randf(void);

int main() {
  training_data_t *training_data = training_data_xor_gate;
  test_data_t *test_data = test_data_common;
  model_t model;

  model_init(&model);

  printf("test before training\n");
  test(&model, test_data);

  printf("starting training\n");
  train(&model, training_data);
  
  printf("test after training\n");
  test(&model, test_data);

  return 0;
}

float randf(void) {
  return (float)rand() / RAND_MAX;
}

void model_init(model_t *model) {
  srand(time(NULL));
  for(int i = 0; i < N_NEURON; i++) {
    for(int j = 0; j < INPUT_DIMENTION; j++) {
      model->weights[i][j] = randf();
    }
    model->biases[i] = randf();
  }
}

float sigmoid(float x) {
  float e = 2.71828f;
  return (1.0f / (1 + powf(e, -x)));
}

float predict_one(model_t *model, float x1, float x2) {
  float y_predict_n1 = 0.0f;
  float y_predict_n2 = 0.0f;
  float y_predict_n3 = 0.0f;

  /*
   * forward pass; for each of the test input -
   * step-1: input x1 and x2 are passed to neuron-1 --> y_predict_n1
   * step-2: input x1 and x2 are passed to neuron-2 --> y_predict_n2
   * step-3: y_predict_n1 and y_predict_n2 passed to neuron-3 --> y_predict_n3
   * */

  y_predict_n1 = sigmoid(x1*model->weights[0][0] +
      x2*model->weights[0][1] +
      model->biases[0]);
  y_predict_n2 = sigmoid(x1*model->weights[1][0] +
      x2*model->weights[1][1] +
      model->biases[1]);
  y_predict_n3 = sigmoid(y_predict_n1*model->weights[2][0] +
      y_predict_n2*model->weights[2][1] +
      model->biases[2]);

  return y_predict_n3;
}

void test(model_t * model, test_data_t *test_data) {
  float y = 0.0f;

  for (int i = 0; i < n_test; i++) {
    float x1 = test_data[i].x1;
    float x2 = test_data[i].x2;

    y = predict_one(model, x1, x2);

    printf("x1: %1.1f\tx2: %1.1f\ty: %1.1f\n", x1, x2, y);
  }
}

float cost(model_t *model, training_data_t *training_data) {
  float mse = 0.0f;

  for(int i = 0; i < n_train; i++) {
    float x1 = training_data[i].x1;
    float x2 = training_data[i].x2;
    float y = training_data[i].y;
  
    float y_predict = predict_one(model, x1, x2);
    float diff = y - y_predict;

    mse += diff * diff;
  }
  return mse / n_train;
}
void copy_model(model_t *model, model_t *copy) {
  for(int i = 0; i < N_NEURON; i++) {
    for(int j = 0; j < INPUT_DIMENTION; j++) {
      copy->weights[i][j] = model->weights[i][j];
    }
    copy->biases[i] = model->biases[i];
  }
   
}
void train(model_t *model, training_data_t *training_data) {
  for(int i = 0; i < n_epoch; i++) {
    float cost_current = cost(model, training_data);

    if(cost_current < acceptable_error) {
      printf("target accuracy achieved after %d epochs\n", i);
      return;
    } 
    
    if(!(i%1000)) {
      printf("iter: %4d\t[cost: %1.4f]\n", i, cost_current);
    }

    /*
     * for each of the weights and biases, calculate cost after nudge 
     * EACH OF THEM -- SEPARATELY!
     * lets create local copy of model, apply delta to one parameter at a time,
     * calculat cost, get the resulting diff to apply,
     * reset back model, apply delta to next one -- repeat!
     * */

    model_t model_copy; //temporary copy to find cost with nudges applied
    model_t model_gradients; //to store gradients to apply for each parameter

    for(int i = 0; i < N_NEURON; i++) {
      for(int j = 0; j < INPUT_DIMENTION; j++) {
        copy_model(model, &model_copy);
        model_copy.weights[i][j] += eps;
        model_gradients.weights[i][j] = ((cost(&model_copy, training_data) - cost_current) / eps) * learning_rate;
      }
      copy_model(model, &model_copy);
      model_copy.biases[i] += eps;
      model_gradients.biases[i] = ((cost(&model_copy, training_data) - cost_current) / eps) * learning_rate;
    }

    //apply gradients to all model parameters
    for(int i = 0; i < N_NEURON; i++) {
      for(int j = 0; j < INPUT_DIMENTION; j++) {
        model->weights[i][j] -= model_gradients.weights[i][j];
      }
      model->biases[i] -= model_gradients.biases[i];
    }
  }
  return;
}
