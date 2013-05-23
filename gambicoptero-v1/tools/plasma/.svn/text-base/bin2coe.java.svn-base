

import java.io.BufferedInputStream;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;

public class bin2coe {


	public static void main(String[] args) {
		try {
            int widthBytes = Integer.parseInt(args[0]);
            int depth = Integer.parseInt(args[1]);
            final int size = widthBytes * depth;

			String in = args[2];
			String out = in.substring(0, in.length()-3)+"coe";
			
			BufferedInputStream bin = new BufferedInputStream(new FileInputStream(new File(in))); 
			BufferedWriter wr = new BufferedWriter(new FileWriter(new File(out)));
			
			wr.write("MEMORY_INITIALIZATION_RADIX=16;\n");
			wr.write("MEMORY_INITIALIZATION_VECTOR=\n");
			
            for(int i = 0; i < size; ++i) {
				if(bin.available() > 0)
				    wr.write( String.format("%02x", bin.read()) );
				else
				    wr.write("00");

				if(i == size-1) // end of contents
					wr.write(";\n");
				else
                    if((i+1) % widthBytes == 0) // end of memory line
                        wr.write(",\n");
			}
			
			bin.close();
			
			wr.flush();
			wr.close();

		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

	}

}
