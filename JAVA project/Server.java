package cse3040fp;

import java.io.*;
import java.net.*;
import java.util*;

public class Server {
	static Comparator<String> comparator = (s1, s2) -> (s1.toLowerCase()).compareTo(s2.toLowerCase());
	TreeMap<String, String> title_author;
	TreeMap<String, String> title_ID;
	
	Server(){
		title_author = new TreeMap<>(comparator);
		title_ID = new TreeMap<>(comparator);
		Collections.synchronizedMap(title_author);
		Collections.synchronizedMap(title_ID);
	}
	
	public void printbooks() {
		for(Map.Entry<String, String> entry : title_author.entrySet()) {
			System.out.println("[" + entry.getKey() + ", " + entry.getValue() + "]");
		}
		
		for(Map.Entry<String, String> entry : title_ID.entrySet()) {
			System.out.println("[" + entry.getKey() + ", " + entry.getValue() + "]");
		}
	}
	
	public void getLibraryData() throws IOException{
		ArrayList<String> L1 = new ArrayList<>(); // titles
		ArrayList<String> L2 = new ArrayList<>(); // authors 
		ArrayList<String> L3 = new ArrayList<>(); // UserIDs
		BufferedReader br = new BufferedReader(new FileReader("books.txt"));
		while(true) {
			String line = br.readLine();
			if(line == null) break;
			String data[] = line.split("\t");
			L1.add(data[0]); L2.add(data[1]); L3.add(data[2]);
		}
		br.close();
		
		for(int i = 0; i < L1.size(); i++) {
			title_author.put(L1.get(i), L2.get(i));
			title_ID.put(L1.get(i), L3.get(i));
		}
	}
	
	public void updateLibrary() throws IOException{
		FileOutputStream fout = new FileOutputStream("books.txt");
		String data = "";
		int cnt = 0;
		ArrayList<String> L1 = new ArrayList<>(); // titles
		ArrayList<String> L2 = new ArrayList<>(); // authors 
		ArrayList<String> L3 = new ArrayList<>(); // UserIDs
		for(Map.Entry<String, String> entry : title_author.entrySet()) {
			L1.add(entry.getKey());
			L2.add(entry.getValue());
			cnt++;
		}
		for(Map.Entry<String, String> entry : title_ID.entrySet()) {
			L3.add(entry.getValue());
		}
		
		for(int i = 0; i < cnt; i++) {
			data += L1.get(i) + "\t" + L2.get(i) + "\t" + L3.get(i) + "\n";
		}
		
		byte[] bytes = data.getBytes();
		fout.write(bytes);
		fout.close();
	}
	
	public void start(String port_number) {
		ServerSocket serverSocket = null;
		Socket socket = null;
		try {
			serverSocket = new ServerSocket(Integer.parseInt(port_number));
			System.out.println("Library[Server] has opened.");
			
			// read book.txt Database
			try {
				getLibraryData();
			}catch(IOException ie) {
				System.out.println("Cannot open file.");
			}

			while(true) {
				socket = serverSocket.accept();
				ServerReceiver thread = new ServerReceiver(socket);
				thread.start();
			}
		} catch(Exception e) { e.printStackTrace(); }
	}
	
	
	public static void main(String args[]) {
		if(args.length != 1) {
			System.out.println("Please give the port number as an arument.");
			System.exit(0);
		}
		try {
			new Server().start(args[0]); // args[0] is port number
		} catch(Exception e) { }
	} // main
	
	
	class ServerReceiver extends Thread{
		Socket socket;
		DataInputStream in;
		DataOutputStream out;
		ServerReceiver(Socket socket){
			this.socket = socket;
			try {
				in = new DataInputStream(socket.getInputStream());
				out = new DataOutputStream(socket.getOutputStream());
			} catch(IOException e) { e.printStackTrace(); }
		}
		
		// ServerReceiver-run()
		public void run() {
			String title, author, UserID, cmd, target;
			boolean isFind, isExist;
			try {
				System.out.println("[" + in.readUTF() + "] has enterd.");
				while(in != null) {
					//printbooks();
					String[] line = in.readUTF().split("\t");
					title = line[0];
					author = line[1];
					UserID = line[2];
					cmd = line[3];
					target = line[4];
					isFind = false;
					isExist = false;
					//System.out.println("title = " + title);
					//System.out.println("author = " + author);
					//System.out.println("UserID = " + UserID);
					//System.out.println("cmd = " + cmd);
					//System.out.println("\n");
					
					if(cmd.equals("add")) {
						// check if already book exists
						Set<String> keys = title_author.keySet();
						for(String key : keys) {
							if(key.equalsIgnoreCase(title)) {
								out.writeUTF("The book already exists in the list.");
								isExist = true;
								break;
							}
						}
						
						// insert new book
						if(!isExist) {
							title_author.put(title, author);
							title_ID.put(title, "-");
						}
						
					}
					
					else if(cmd.equals("borrow")) {
						// check if already book exists
						String original_title = null;
						Set<String> keys = title_author.keySet();
						for(String key : keys) {
							if(key.equalsIgnoreCase(title)) {
								if(title_ID.get(key).equals("-")) {
									original_title = key;
									isFind = true;
									break;
								}
							}
						}
						
						if(isFind) {
							title_ID.put(title, UserID);
							out.writeUTF("You borrowed a book. - " + original_title);
						}
						else {
							out.writeUTF("The book is not available.");
						}
					}// BORROW
					
					else if(cmd.equals("return")) {
						String original_title = null;
						Set<String> keys = title_author.keySet();
						for(String key : keys) {
							if(key.equalsIgnoreCase(title)) {
								if(title_ID.get(key).equals(UserID)) {
									original_title = key;
									isFind = true;
									break;
								}
							}
						}
						
						if(isFind) {
							title_ID.put(title, "-");
							out.writeUTF("You returned a book. - " + original_title);
						}
						else {
							out.writeUTF("You did not borrow the book.");
						}
					} // RETURN
					
					else if(cmd.equals("info")) {
						int cnt = 0;
						ArrayList<String> L1 = new ArrayList<>(); // title list
						ArrayList<String> L2 = new ArrayList<>(); // author list
						Set<String> keys = title_ID.keySet();
						for(String key : keys) {
							if(title_ID.get(key).equals(UserID)) {
								cnt++;
								L1.add(key);
								L2.add(title_author.get(key));
							}
						}
						
						if(cnt == 0) {
							out.writeUTF("You are currently borrowing 0 books:");
						}
						else {
							String data = "You are currently borrowing " + Integer.toString(cnt) + " books:";
							for(int i = 0; i < cnt; i++) {
								data += "\n" + Integer.toString(i + 1) + ". " + L1.get(i) + ", " + L2.get(i);
							}
							out.writeUTF(data);
						}
						
					}// INFO
					
					else if(cmd.equals("search")) {
						// target is pattern string
						String data = "";
						String data2 = "";
						int cnt = 0;
						String pattern = target.toLowerCase();
						for(Map.Entry<String, String> entry : title_author.entrySet()) {
							String k = entry.getKey().toLowerCase();
							String v = entry.getValue().toLowerCase();
							if(k.contains(pattern) || v.contains(pattern)) {
								cnt++;
								data2 += "\n" + Integer.toString(cnt) + ". " + entry.getKey() + ", " + entry.getValue();
							}
						}
						data = "Your search matched " + Integer.toString(cnt) + " results.";
						if(cnt != 0) data += data2;
						out.writeUTF(data);
						
					}// SEARCH
					updateLibrary();
					getLibraryData();
				}// WHILE
			} catch(IOException e) { e.printStackTrace(); }
			finally {

			}
		}
	}
	
}
