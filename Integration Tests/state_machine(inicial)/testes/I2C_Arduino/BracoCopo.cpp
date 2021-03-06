#include "BracoCopo.h"

BracoCopo::BracoCopo(){}

BracoCopo::BracoCopo(int servoPulso, int servoGarra, int sharpGarra, Motor m){
  pulso.attach(servoPulso);
  garra.attach(servoGarra);
  pulso.write(0);
  this->sharpGarra = sharpGarra;

  motorBraco.setPinFrente(m.getPinFrente());
  motorBraco.setPinTras(m.getPinTras());
}

BracoCopo::BracoCopo(int servoPulso, int servoGarra, int sharpGarra, int mSharp_D, int mSharp_E, Motor m){
  pulso.attach(servoPulso);
  garra.attach(servoGarra);
  this->sharpGarra = sharpGarra;
  this->mSharp_D = mSharp_D;
  this->mSharp_E = mSharp_E;

  motorBraco.setPinFrente(m.getPinFrente());
  motorBraco.setPinTras(m.getPinTras());
}

void BracoCopo::attachMotor(Motor m){
  motorBraco.setPinFrente(m.getPinFrente());
  motorBraco.setPinTras(m.getPinTras());
}

int BracoCopo::getSharp(){
    SharpIR SharpIR(sharpGarra, 1080);
    byte n = 20;
    int media, desvio;
    int x[20];
    long unsigned soma=0;
    for(int i=0;i<n;i++){
        x[i] = SharpIR.distance();
        soma += x[i];  // this returns the distance to the object you're measuring
    }
    media = (soma/n);
    soma = 0;
    for (int i =0;i< n; i++) {
      soma += (x[i]-media)*(x[i]-media);
    }

    desvio = soma/n;

    if (desvio > VALID_SHARP || media > 80) {
      return -1; // é ruido
    } else {
      return media; // n é ruido
    }
}

void BracoCopo::tryGetTerrine(){
  do {
    motorBraco.moveMotor(255, 1);
  } while(isFDC(FDC_FRENTE) && analogRead(MSH_GARRA_D) > ANALOG_SENSOR_COPO);
  motorBraco.moveMotor(0,1);
  garra.write(POSICAO_GARRA_FECHADA);

}

void BracoCopo::recolherBraco(){
  do {
    motorBraco.moveMotor(255, 0);
  } while(isFDC(FDC_TRAS));
}