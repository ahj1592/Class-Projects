package cse3040fp;

import java.io.*;
import java.util.*;
import java.net.*;

public class Client {
	static class ClientSender extends Thread {
		Socket socket;
		DataOutputStream out;
		String title;
		String author;
		String UserID;
		String cmd;

		ClientSender(Socket socket, String UserID) {
			this.socket = socket;
			this.UserID = UserID;
			try {
				out = new DataOutputStream(socket.getOutputStream());
			} catch (Exception e) { }
		}

		@SuppressWarnings("all")
		public void run() {
			Scanner sc = new Scanner(System.in);
			try {
				out.writeUTF(UserID);
				while (out != null) {
					System.out.print(UserID + ">> ");
					String cmd = sc.nextLine();
					
					if (cmd.equals("add")) {
						System.out.print("add-title> ");
						String newTitle = sc.nextLine();
						if (newTitle.length() == 0)
							continue;
						
						System.out.print("add-author> ");
						String newAuthor = sc.nextLine();
						if (newAuthor.length() == 0)
							continue;
						
						out.writeUTF(newTitle + "\t" + newAuthor + "\t" + UserID + "\t" + cmd + "\t" + "dummy");
						try {
							Thread.sleep(100);
						} catch(InterruptedException e) { }
						
					} else if (cmd.equals("borrow")) {
						System.out.print("borrow-title> ");
						String borrowTitle = sc.nextLine();
						
						if (borrowTitle.length() == 0)
							continue;
						
						out.writeUTF(borrowTitle + "\t" + "dummy" + "\t" + UserID + "\t" + cmd + "\t" + "dummy");
						try {
							Thread.sleep(100);
						} catch(InterruptedException e) { }

			
					} else if (cmd.equals("return")) {
						System.out.print("return-title> ");
						String returnTitle = sc.nextLine();
						if (returnTitle.length() == 0)
							continue;
						out.writeUTF(returnTitle + "\t" + "dummy" + "\t" + UserID + "\t" + cmd + "\t" + "dummy");
						try {
							Thread.sleep(100);
						} catch(InterruptedException e) { }

					} else if (cmd.equals("info")) {
						// show UserID's borrow list
						out.writeUTF("dummy" + "\t" + "dummy" + "\t" + UserID + "\t" + cmd + "\t" + "dummy");
						try {
							Thread.sleep(100);
						} catch(InterruptedException e) { }
						
					} else if (cmd.equals("search")) {
						String target = null;
						while (true) {
							System.out.print("search-string> ");
							target = sc.nextLine();
							if (target.length() == 0)
								break;
							if (target.length() < 3) {
								System.out.println("Search-string must be longer than 2 characters.");
							}
							else break;
						}
						if(target.length() != 0)
							out.writeUTF("dummy" + "\t" + "dummy" + "\t" + UserID + "\t" + cmd + "\t" + target);
						try {
							Thread.sleep(100);
						} catch(InterruptedException e) { }
					} else {
						System.out.println("[available commands]");
						System.out.println("add: add a new book to the list of books.");
						System.out.println("borrow: borrow a book from the library.");
						System.out.println("return: return a book to the library.");
						System.out.println("info: show list of books I am currently borrowing.");
						System.out.println("search: search for books.");
					}
					
				}//while()
			} catch (IOException ie) { }
		}
	}
		static class ClientReceiver extends Thread {
			Socket socket;
			DataInputStream in;
			String UserID;

			ClientReceiver(Socket socket, String UserID) {
				this.socket = socket;
				this.UserID = UserID;
				try {
					in = new DataInputStream(socket.getInputStream());

				} catch (IOException e) { }
			}

			public void run() {
				while(in != null) {
					try {
						System.out.println(in.readUTF());
					} catch( IOException e) { }
				}
			}

		}

		public static void main(String args[]) {
			Socket socket = null;
			if (args.length != 2) {
				System.out.println("Please give the IP address and port number as arguments.");
				System.exit(0);
			}

			try {
				String serverIp = args[0];
				int port_number = Integer.parseInt(args[1]);
				socket = new Socket(serverIp, port_number);

			} catch (ConnectException ce) {
				System.out.println("Connection establishment failed.");
				System.exit(0);
			} catch (Exception e) { }

			// Input UserID
			String UserID = null;
			boolean valid = true; // if UserID is valid
			Scanner sc = new Scanner(System.in);
			while (true) {
				valid = true;
				System.out.print("Enter userID>> ");
				UserID = sc.nextLine();

				// Check if UserID has only alphabets
				char[] chars = UserID.toCharArray();

				for (char c : chars) {
					if (!Character.isLetter(c)) {
						System.out.println("UserID must be a single word with lowercase alphabets and numbers.");
						valid = false;
						break;
					}
				}
				if (valid)
					break;
			}
			// sc.close();

			System.out.println("Hello " + UserID + "!");
			Thread sender = new Thread(new ClientSender(socket, UserID));
			Thread receiver = new Thread(new ClientReceiver(socket, UserID));
			sender.start();
			receiver.start();
			
		}
	}

