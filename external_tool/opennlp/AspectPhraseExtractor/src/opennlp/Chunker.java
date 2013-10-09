package opennlp;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import opennlp.tools.chunker.ChunkerME;
import opennlp.tools.chunker.ChunkerModel;

public class Chunker {

	/**
	 * This class utilize Chunking function in OpenNLP to chunk sentences.
	 * 
	 * @author Wei Zhang
	 * @date 2013/10/9
	 */
	private static String model_path = "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF"+
			  "/external_tool/opennlp/AspectPhraseExtractor/model/en-chunker.bin";
	private ChunkerME chunker;
	
	public Chunker(){
		System.out.println("Create an instance of class 'Chunker'.");
		
		InputStream modelIn = null;
		ChunkerModel model = null;

		try {
			System.out.println("Loading model...");
			modelIn = new FileInputStream(model_path);
			model = new ChunkerModel(modelIn);
			chunker = new ChunkerME(model);
			System.out.println("Model loaded success!");
		} catch (IOException e) {
		  // Model loading failed, handle the error
		  e.printStackTrace();
		  System.out.println("Error in class 'Chunker'.");
		  System.exit(1);
		} finally {
		  if (modelIn != null) {
		    try {
		      modelIn.close();
		    } catch (IOException e) {
		    	System.out.println("Error in class 'Chunker'.");
				  System.exit(1);
		    }
		  }
		}
	}
	
	public String[] run(String sent[], String pos[]){
		String tags[] = null;
		
		tags = chunker.chunk(sent, pos);
		return tags;
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println("Testing in the chunker program.");
		
		String sent[] = new String[] { "Rockwell", "International", "Corp.", "'s",
			    "Tulsa", "unit", "said", "it", "signed", "a", "tentative", "agreement",
			    "extending", "its", "contract", "with", "Boeing", "Co.", "to",
			    "provide", "structural", "parts", "for", "Boeing", "'s", "747",
			    "jetliners", "." };

		String pos[] = new String[] { "NNP", "NNP", "NNP", "POS", "NNP", "NN",
			    "VBD", "PRP", "VBD", "DT", "JJ", "NN", "VBG", "PRP$", "NN", "IN",
			    "NNP", "NNP", "TO", "VB", "JJ", "NNS", "IN", "NNP", "POS", "CD", "NNS",
			    "." };
		
		Chunker chunker = new Chunker();
		
		String tags[] = chunker.run(sent, pos);
		for (String tag: tags){
			System.out.println(tag);
		}
		System.out.println(tags.length);
	}

}
