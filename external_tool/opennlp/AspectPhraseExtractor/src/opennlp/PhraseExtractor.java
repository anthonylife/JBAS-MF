package opennlp;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class PhraseExtractor {

	/**
	 * This class convert raw input text into tagged text data.
	 * 
	 * @author Wei Zhang
	 * @date 2013/10/9
	 */
	private Chunker chunker;
	private Postagger postagger;
	private Tokener tokener;
	
	public PhraseExtractor() throws FileNotFoundException{
		chunker = new Chunker();
		postagger = new Postagger();
		tokener = new Tokener();
		//System.out.println("Model loaded success!");
	}

	// Convert sentence to segments
/*	public final String [] tokenize(String raw_text, String sep_char){
		String [] tokens = raw_text.split(sep_char);
		return tokens;
	}*/
	
	// Tag the segments of the sentence
	public void tagging(String paths[]) throws IOException{
		final int sep_num = paths.length/2;
		FileReader rd;
		BufferedReader reader;
		FileWriter writer;
		
		String [] templine;
		String [] tokens;
		String [] pos_tags;
		String [] chunk_tags;
		String readline = null;	
		for (int i=0; i<sep_num; i++){
			rd = new FileReader(paths[i]);
			reader = new BufferedReader(rd);
			writer = new FileWriter(paths[i+sep_num]);
						
			while((readline = reader.readLine()) != null){
				templine = readline.split("::");
				if (templine.length != 2){
					System.out.println(templine[0]);
					System.out.println(templine[1]);
					System.out.println(templine[2]);
					System.out.println("Raw text format error.");
					continue;
					//System.exit(1);
				}
				
				//Tokenize the sentence
				tokens = tokener.tokenize(templine[1]);
				pos_tags = postagger.run(tokens);
				//for (String tag: pos_tags){
				//	System.out.println(tag);
				//}
				//System.in.read();cd
				
				//Chunking the sentence
				chunk_tags = chunker.run(tokens, pos_tags);
				//for (String tag: chunk_tags){
				//	System.out.println(tag);
				//}
				//System.in.read();
				
				//Formating output
				writer.write(templine[0]+"::"+tokens[0]+"_"+pos_tags[0]+"_"+chunk_tags[0]);
				for (int j=1; j<tokens.length; j++){
					writer.write(" "+tokens[j]+"_"+pos_tags[j]+"_"+chunk_tags[j]);
				}
				writer.write("\n");
			}
			reader.close();
			writer.close();
		}
	}
	
	public static void main(String[] args) throws IOException {
		// Setting the paths of the files to be processed.
		final String old_cellar_tracker_text_file = "/home/anthonylife/Doctor/Code/MyPaperCode" +
				"/JBAS-MF/data/CellarTracker/filter/reviewid_text.dat";
		final String old_amazon_food_text_file = "/home/anthonylife/Doctor/Code/MyPaperCode" +
				"/JBAS-MF/data/Amazon-Food/filter/reviewid_text.dat";
		final String old_amazon_movie_text_file = "/home/anthonylife/Doctor/Code/MyPaperCode" +
				"/JBAS-MF/data/Amazon-Movies/filter/reviewid_text.dat";
		// Setting the paths of the new files getting outputdata.
		final String new_cellar_tracker_text_file = "/home/anthonylife/Doctor/Code/MyPaperCode" +
				"/JBAS-MF/data/CellarTracker/filter/reviewid_tagged_text.dat";
		final String new_amazon_food_text_file = "/home/anthonylife/Doctor/Code/MyPaperCode" +
				"/JBAS-MF/data/Amazon-Food/filter/reviewid_tagged_text.dat";
		final String new_amazon_movie_text_file = "/home/anthonylife/Doctor/Code/MyPaperCode" +
				"/JBAS-MF/data/Amazon-Movies/filter/reviewid_tagged_text.dat";
		
		String paths[] = {old_cellar_tracker_text_file, old_amazon_food_text_file,
				old_amazon_movie_text_file, new_cellar_tracker_text_file,
				new_amazon_food_text_file, new_amazon_movie_text_file};
		
		PhraseExtractor extractor = new PhraseExtractor();
		extractor.tagging(paths);
	}

}
