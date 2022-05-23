/*
*
* Test: 
* 	java RollerCoaster 2 2 2 5000
* Should give :
*	Car starts to load
*	Passenger 0 and 1 take ride
*	Car starts around the track
*	(wait 5 seconds)
*	Car unloads
*	Passenger 0 and 1 return from ride
*	Car starts to load
*	Passenger 0 and 1 take ride
*	Car starts around the track
* 	(wait 5 seconds)
*	Car unloads
*	Passenger 0 and 1 return from ride
*	All passengers are done
*
* Test 2:
*	java RollerCoaster 10 20 2 2000
*/


import java.util.Random;
import java.util.ArrayList;

class Shared {
	static long startT = System.currentTimeMillis();

	public static int timeStamp() {
		return (int) (System.currentTimeMillis() - startT)/1000;
	}
}

class Passenger extends Thread {

	int pid;
	int numRides;
	Random r;
	Monitor m;

	public Passenger(int pid, int numRides, Random r, Monitor m) {
		this.pid = pid;
		this.numRides = numRides;	
		this.r = r;
		this.m = m;
	}

	@Override
	public void run() {
		
		for (int i = 0; i < numRides; i++) {
			// Do stuff
			try {
				Thread.sleep(2000+r.nextInt(1000));
			} catch (Exception e) {}

			System.out.printf("%d: Passenger %d takes ride\n", Shared.timeStamp(), pid);
			m.takeRide();
			System.out.printf("%d: Passenger %d returns from ride\n", Shared.timeStamp(), pid);
		}


	}

}

class Car extends Thread {

	int rideTime;
	int c;
	Monitor m;
	int run;

	public Car(int rideTime, int capacity, Monitor m) {
		this.rideTime = rideTime;
		this.c = capacity;
		this.m = m;
		this.run = 1;
	}

	@Override
	public void run() {

		while(run == 1) {

			System.out.printf("%d: Car starts to load\n", Shared.timeStamp());

			m.load(c);

			System.out.printf("%d: Car starts around the track\n", Shared.timeStamp());

			// Ready to go

			try {
				Thread.sleep(rideTime);
			} catch (Exception e) {}

			System.out.printf("%d: Car unloads\n", Shared.timeStamp());
			m.unload();

			try {
				Thread.sleep(2000);
			} catch (Exception e) {}

		}

	}

	public void kill() {
		this.run = 0;
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

		// Another solution: have each passenger decrease waitingP by 1 and wait here untill
		// waitingP == 0 then change takeRide to 0
		
		takenRide = 0;
		notifyAll(); // Wake those sleeping in round two at beginning of takeRide()

		try {
			while(waitingPassengers < this.c) wait();
		} catch (Exception e) {}
		

	}

	public synchronized void unload() {

		takenRide = 1;
		waitingPassengers -= c;
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
		} catch (Exception e) {}


	}


}


public class RollerCoaster {

	public static void main (String[] args) {


		int c = Integer.parseInt(args[0]);
		int n = Integer.parseInt(args[1]);
		int numRides = Integer.parseInt(args[2]);
		int rideTime = Integer.parseInt(args[3]);

		Random rand = new Random();
		Monitor m = new Monitor(c);

		ArrayList<Passenger> passengerList = new ArrayList<Passenger>();

		Car carThread = new Car(rideTime, c, m);

		carThread.start();

		for (int i = 0; i < n; i++) {
			passengerList.add(new Passenger(i, numRides, rand, m));
			passengerList.get(i).start();
		}


		for (Passenger p : passengerList) {
			try {
				p.join();
			} catch (Exception e) {}
		}

		System.out.println(Shared.timeStamp() + ": All passengers done!");

		carThread.kill();


	}

}

