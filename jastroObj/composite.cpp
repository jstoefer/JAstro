#include "composite.h"

Composite::Composite(Horoscope *h1, Horoscope *h2) {

    this->n_planets = 16;   // including Lilith, Chiron

    this->h = new Horoscope("Komposit");
    this->h1 = h1;
    this->h2 = h2;
    this->calculate();
}

void Composite::setH1(Horoscope * horoscope) {
    this->h1 = horoscope;
    if ((this->h1 != NULL) && (this->h2 != NULL)) {
        this->calculate();
    }
}

void Composite::setH2(Horoscope * horoscope) {
    this->h2 = horoscope;
    if ((this->h1 != NULL) && (this->h2 != NULL)) {
        this->calculate();
    }
}

void Composite::calculate() {

    // qDebug() << "Composite::calculate(), begin";

    /* calculate planetary half sums   */
     for (int i=0; i < n_planets; i++) {                 /* iterate over all planets     */
         int direction = 1;
         float d = h1->planet[i] - h2->planet[i];
         if (d < 0) {
             direction *= -1;    /* h2->p > h1->p, half sum is smaller than h1->p        */
         }
         d = abs(d);
         if (d > 180) {
             d = 360-d;   /* get smaller half sum     */
             direction *= -1;
         }
         this->h->planet[i] = h2->planet[i] +direction*d/2;

         /* check for negative angles                   */
         if (h->planet[i] < 0) {
             h->planet[i] += 360;
         }

         /* check for angles > 360°                     */
         if (h->planet[i] > 360) {
             h->planet[i] -= 360;
         }

         // qDebug() << "ipl = " << i << ",  comp. = " << h->planet[i];

     }

     /* calculate cusp half sums      */
     for (int i=1; i < 7; i++) {                 /* iterate over cusps                  */
         int direction = 1;
         double d = h1->cusp[i] - h2->cusp[i];
         if (d < 0) {
             direction *= -1;    /* h2->p > h1->p, half sum i ssmaller than h1->p       */
         }
         d = abs(d);
         if (d > 180) {
             d = 360-d;   /* get smaller half sum     */
             direction *= -1;
         }
         this->h->cusp[i] = h2->cusp[i] + direction*d/2;
         if (this->h->cusp[i] > 360) {
             this->h->cusp[i] -= 360;
         } else if (this->h->cusp[i] < 0) {
             this->h->cusp[i] += 360;
         }
         this->h->cusp[i+6] = 180 + this->h->cusp[i];
         if (this->h->cusp[i+6] > 360) {
             this->h->cusp[i+6] -= 360;
         } else if (this->h->cusp[i+6] < 0) {
             this->h->cusp[i+6] += 360;
         }
         // if ((i == 1) || (i == 2) || (i == 3) ) {
         //    qDebug() << "H" << i << " 1 = " << h1->cusp[i] << ", H" << i << " 2 = " << h2->cusp[i] << "d = " << d;
         //    qDebug() << "H" << i <<" comp. = " << h->cusp[i];
         // }
     }
     /* Re - order cusps so that they are arranged anti - clockwise
        beginning with MC.                                                           */
     for (int k= 0;  k< 5; k++) {
         /* calculate cusp index starting with H10    */
         int i = (k+9) % 11 +1;
         int j = (k+10) % 11 +1;
         double d = this->h->cusp[j] - this->h->cusp[i];
         // qDebug() << "i =" << i << ", j = " << j << ", d = " << d;
         if ( (d < 0) && (abs(d) < 180)) {
             int l = (j+5)%11 +1;
             // qDebug() << "swapping cusp["<< j << "] with cusp[" << l <<"]" ;
             double a = this->h->cusp[j];
             this->h->cusp[j] = this->h->cusp[l];
             this->h->cusp[l] = a;
         }
     }

     /* calculate aspects                                   */
     h->setLastPlanet(SE_CHIRON);     // make configurable later ..
     h->update();

     /* reset retrograde flaggs, because they do not make any sense              */
     for (int ipl = 0; ipl < SE_NPLANETS; ipl++) {
         h->retrograde[ipl] = FALSE;
     }
}
