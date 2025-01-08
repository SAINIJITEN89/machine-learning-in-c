#include <stdio.h>
#include <math.h>

/*
 * function to model 
 * y = x*w;
 *
 * y: output
 * x: input
 * w: model parameter
 * 
 * (x,y,w) are real numbers
 *
 * training data = [] {x, y} pairs
 *
 * inference : x --> trained model (value of w) --> y 
 *
 * training process : []{x, y} --> learning algoritham 
 * --> learned value of model parameters w 
 *
 * learning algorithm : learn value of "w" that satisfies all pairs of {x, y}
 * */

#ifdef TRUE
#else
#endif

const float x = +1.0f;
const float y = -2.0f;
const float acceptable_error = 0.001;
const float small_delta = 0.1;
const int n_iter = 1000;

float w = 0.5f;   // todo: check effect with different initial values

int main() {
  int i = 0;

  while(++i < n_iter) {
    float y_predict = x * w;
    if (fabs(y_predict - y) < acceptable_error) break; 

    if (y_predict > y) {
      w -= small_delta;
    } else {
      w += small_delta;
    }
    printf("iter: %d\tw: %f\n", i, w);
  }

  float test_x = 3.0;
  printf("\ntraining:\tx: %1.1f --> y: %1.1f\n", x, y);
  printf("test: \t\tx: %1.1f --> y: %1.1f\n", test_x, test_x * w);
}
