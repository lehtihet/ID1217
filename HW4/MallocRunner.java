import java.util.Random;

class Shared {
	static Random rand = new Random();
}

class Malloc {

	int freePages;

	public Malloc(int freePages) {
		this.freePages = freePages;
	}


	public synchronized void request(int amount) {

		// Delay untill amount free pages are available

		System.out.printf("Requested: %d, Current pool: %d\n", amount, freePages);

		try {
			while (freePages < amount) this.wait(); 
			freePages -= amount;
		} catch (Exception e) {}
		
	}

	public synchronized void release(int amount) {
		// Release amount to the free pool

		freePages += amount;

		System.out.printf("Releasing: %d, Current pool: %d\n", amount, freePages);

		this.notifyAll();

	}

}

class RequestTest extends Thread {

	Malloc m;
	Random r;

	public RequestTest(Malloc m) {
		this.m = m;
		this.r = Shared.rand;
	}

	@Override
	public void run() {

		int i;

		while(true) {

			try {
				Thread.sleep(r.nextInt(2000));
			} catch (Exception e) {}

			i = r.nextInt(100);

			m.request(i);

		}

	}

}

class ReleaseTest extends Thread {

	Malloc m;
	Random r;

	public ReleaseTest(Malloc m) {
		this.m = m;
		this.r = Shared.rand;
	}

	@Override
	public void run() {

		int i;

		while(true) {

			try {
				Thread.sleep(r.nextInt(2000));
			} catch (Exception e) {}

			i = r.nextInt(100);

			m.release(i);


		}

		

	}

}

public class MallocRunner {

	public static void main (String[] args) {

		Malloc m = new Malloc(0);

		RequestTest req1 = new RequestTest(m);
		RequestTest req2 = new RequestTest(m);
		RequestTest req3 = new RequestTest(m);

		ReleaseTest rel1 = new ReleaseTest(m);
		ReleaseTest rel2 = new ReleaseTest(m);
		ReleaseTest rel3 = new ReleaseTest(m);

		req1.start();
		req2.start();
		req3.start();

		rel1.start();
		rel2.start();
		rel3.start();


		

	}
}

