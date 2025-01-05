#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

float randf(void);
float cost(float, float);

  int x1 = 1;
  int x2 = 4;
  float y_actual = 9.0; /* x1 * 3 + x2 * 1.5 */
  float eps = 0.01;

int main () {
  srand(time(NULL));

  float w1, w2;
  w1 = randf() * 1000.0f;
  w2 = randf() * 1000.0f;

  float w1_initial = w1;
  float w2_initial = w2;

//  printf("w1: %f\n", w1);
//  printf("w2: %f\n", w2);
//  printf("cost: %f\n", cost(w1, w2));
//  printf("cost w1+eps: %f\n", cost(w1 + eps, w2));
//  printf("cost w1-eps: %f\n", cost(w1 - eps, w2));
//  printf("cost w2+eps: %f\n", cost(w1, w2 + eps));
//  printf("cost w2-eps: %f\n", cost(w1, w2 - eps));


  int nTrainIter = 1000;
  int iter = 0;
  while(cost(w1,w2) > 0.01f) {
  // lets nudge the weights accordingly
  if (cost(w1+eps, w2) < cost(w1, w2)) w1 += eps;
  else if (cost(w1-eps, w2) < cost(w1, w2)) w1 -= eps;

  if (cost(w1, w2+eps) < cost(w1, w2)) w2 += eps;
  else if (cost(w1, w2-eps) < cost(w1, w2)) w2 -= eps;
//  printf("after adjusting weights\n");
//  printf("w1: %f\n", w1);
//  printf("w2: %f\n", w2);
  printf("iter: %d\t cost: %f\ty: %f\n", ++iter, cost(w1, w2), x1*w1 + x2*w2);
  }

  printf("nTrainingIterations: %d\n", iter);
  printf("w1_initial: %f\tw1_final: %f\n", w1_initial, w1);
  printf("w2_initial: %f\tw2_final: %f\n", w2_initial, w2);
  // at this point if y is total shit, we want to make it less shitty.
  // why not turn shit into gold? because then in the buggy case, you'll have shit around your neck.'
  // i hope that was demotivating enough.
  // but anyways - lets see how to adjust the weights.
  // y has SOME value -- we KNOW the CORRECT value 
  // calculate difference y-expected - y-predicted
  // stei-2; we need to make adjustment to all weights in the direction that improves prediction.
  // for this, lets take each weight and vary it in two directions , keeping rest of the params same, select the direction of change -- positive or negiative for w1 which improves the predicted y.
  // do same for w2 separately.
  // appy adjustments to both w7 and w3
   

}

float randf(void) {
  /* range -1 to + 1 */
  //return (((float)rand() / RAND_MAX) * 2.0f) - 1.0f;
  /* range 0 to + 1 */
  return (((float)rand() / RAND_MAX));
}

float cost(float w1, float w2) {
  return fabsf(y_actual - (x1*w1 + x2*w2));
}
