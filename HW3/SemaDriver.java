import java.util.concurrent.*;
import java.util.ArrayList;
import java.util.Random;


class Shared {
	static ArrayList<Semaphore> board = new ArrayList<>();
	static Random rand = new Random();
}


class Student extends Thread {

	int a1, a2, a3;
	int sid, boardSize;

	public Student(int sid) {
		this.sid = sid;
		this.boardSize = Shared.board.size();
	}

	@Override
	public void run() {

		while(true) {

			// Randomly select three apartments
			a1 = Shared.rand.nextInt(boardSize); 
			a2 = Shared.rand.nextInt(boardSize); 
			a3 = Shared.rand.nextInt(boardSize);

			// All apartments have to be different
			if (a1 == a2 || a1 == a3 || a2 == a3) continue;
			

			if (!Shared.board.get(a1).tryAcquire()) { 
				// Sleep
				try {
					Thread.sleep(500);
				} catch (Exception e) {}

				continue;
			}
			if (!Shared.board.get(a2).tryAcquire()) {
				// TryAcquire combined with releasing avoids deadlocks
				// If we can't get an apartment we drop all our locks so no deadlocks can occur
				Shared.board.get(a1).release();
				// Sleep
				try {
					Thread.sleep(500);
				} catch (Exception e) {}

				continue;
			}
			if (!Shared.board.get(a3).tryAcquire()) {
				Shared.board.get(a1).release();
				Shared.board.get(a2).release();
				// Sleep
				try {
					Thread.sleep(500);
				} catch (Exception e) {}

				continue;
			}

			System.out.printf("Student %d finished with apartments %d, %d, %d\n", sid, a1, a2, a3);
			return;

		}
		
	}

}


public class SemaDriver {
	

	public static void main (String[] args) {

		ArrayList<Student> students = new ArrayList<>();

		int n = 100; // Students
		int notices = 120; // Notices should be greater than n
		int i;

		// Initialize board
		for (i = 0; i < notices; i++) 
			Shared.board.add(new Semaphore(3));

		// Create students
		for (i = 0; i < n; i++)
			students.add(new Student(i));

		// Start students
		for (i = 0; i < n; i++) 
			students.get(i).start();
			

	}

}