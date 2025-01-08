#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*
 * function to model 
 * y = x1*w1 + x2*w2 + b;
 *
 * y:       output
 * x1, x2:  input
 * w1, w2:  model parameters
 * b:       bias
 * 
 * training data = known {x1, x2, y}
 *
 * training process : []{x, y} --> learning algoritham --> Model(w1, w2, b) 
 *
 * learning algorithm : find values of model parameters w1, w2 and b, 
 *                      such that it best fits all training data pairs {x, y}
 * */

typedef struct model_params {
  float w1;
  float w2;
  float b;
}model_t;

typedef struct training_data {
  float x1;
  float x2;
  float y;
}training_data_t;

typedef struct test_data {
  float x1;
  float x2;
}test_data_t;

int n_test = 4;
int n_train = 4;
int n_epoch = 50;
float eps = 0.01;
float acceptable_error = 0.001;

training_data_t training_data_or_gate[] = {
  {0, 0, 0},
  {0, 1, 1},
  {1, 0, 1},
  {1, 1, 1}
};

test_data_t test_data_common[] = {
  {0, 0},
  {0, 1},
  {1, 0},
  {1, 1}
};

// can be initialized with random values
model_t model_init = {0.5f, 0.5f, 0.5f};

void test(model_t *, test_data_t *);
void train(model_t *, training_data_t *);

int main() {
  training_data_t *training_data = training_data_or_gate;
  test_data_t *test_data = test_data_common;
  model_t *model = &model_init;

  srand(time(NULL));
  model->w1 = (float)rand() / RAND_MAX;
  model->w2 = (float)rand() / RAND_MAX;
  model->b = (float)rand() / RAND_MAX;

  printf("test before training\n");
  test(model, test_data);

  printf("starting training\n");
  train(model, training_data);
  
  printf("test after training\n");
  test(model, test_data);

  return 0;
}

void test(model_t * model, test_data_t *test_data) {
  for (int i = 0; i < n_test; i++) {
    float x1 = test_data[i].x1;
    float x2 = test_data[i].x2;
    float w1 = model->w1;
    float w2 = model->w2;
    float b = model->b;
  
    float y = x1 * w1 + x2 * w2 + b;
    printf("x1: %1.1f\t\tx2: %1.1f\t-->\ty: %1.1f\n", x1, x2, y);
  }
}

float cost(float w1, float w2, float b, training_data_t *training_data) {
  float mse = 0.0f;

  for(int i = 0; i < n_train; i++) {
    float x1 = training_data[i].x1;
    float x2 = training_data[i].x2;
    float y = training_data[i].y;
  
    float y_predict = x1 * w1 + x2 * w2 + b;
    float diff = y - y_predict;

    mse += diff * diff;
  }
  return mse / n_train;
}

void train(model_t *model, training_data_t *training_data) {
  for(int i = 0; i < n_epoch; i++) {
    float w1 = model->w1;
    float w2 = model->w2;
    float b = model->b;

    float cost_current = cost(w1, w2, b, training_data);

    if(cost_current < acceptable_error) {
      printf("target accuracy achieved after %d epochs\n", i);
      return;
    } else {
      printf("iter: %4d\t[cost: %1.4f]\t model param:\
          {w1: %1.4f\tw2: %1.4f\tb: %1.4f}\n", i, cost_current, w1, w2, b);
    }

    if (cost(w1+eps, w2, b, training_data) < cost_current) {
      w1 += eps;
    } else {
      w1 -= eps;
    }

    if (cost(w1, w2+eps, b, training_data) < cost_current) {
      w2 += eps;
    } else {
      w2 -= eps;
    }

    if (cost(w1, w2, b+eps, training_data) < cost_current) {
      b += eps;
    } else {
      b -= eps;
    }

    model->w1 = w1;
    model->w2 = w2;
    model->b = b;
  }
  return;
}
