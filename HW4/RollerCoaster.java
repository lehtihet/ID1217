

class Passenger extends Thread {

}

class Car extends Thread {

	int passengers;
	int C;
	Monitor m;

	public Car(int capacity, Monitor m) {
		this.c = capacity;
		this.m = m;
	}

	@Override
	public void run() {

		m.load(c);

		m.unload();

	}

}

class Monitor {

	int waitingPassengers;
	int c;
	int takenRide;

	public Monitor(int c) {
		this.c = c;
		this.takenRide = 0;
	}


	public synchronized void load(int c) {

		takenRide = 0;
		notifyAll() // Wake those sleeping in round two at beginning of takeRide()

		try {
			while(waitingPassengers < this.c) wait();
		} catch (Exception e) {}

		// Ready to go
		Thread.sleep(500);
		

	}

	public synchronized void unload() {
		takenRide = 1;
		notifyAll();

	}

	public synchronized void takeRide() {

		try {
			while (waitingPassengers >= this.c || takenRide == 1) wait();
		} catch (Exception e) {}


		waitingPassengers++;
		notifyAll();

		try {
			while(takenRide != 1) wait();
		} Catch (Exception e) {}

		waitingPassengers--;


	}


}


public class RollerCoaster {

	public static void main (String[] args) {



	}

}

/*
* Monitor: Varje passagerare kör takeRide vilket i monitorn kallar på wait() 
* tills takenRide = 1 och ökar passengers i monitorn
* Car kallar load vilket väntar tills passengers=C och som sedan väntar rideTime och 
* sätter takenRide = 1. Efter ett tag sätter car passengers = takenRide = 0 (kanske i unload?).
*
*
*
*
*
*/