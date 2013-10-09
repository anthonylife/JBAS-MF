package opennlp;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import opennlp.tools.postag.POSModel;
import opennlp.tools.postag.POSTaggerME;

public class Postagger {
	/**
	 * This class utilize Postagging function in OpenNLP to tag sentences.
	 * 
	 * @author Wei Zhang
	 * @date 2013/10/9
	 */
	private static String model_path = "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF" +
	  		"/external_tool/opennlp/AspectPhraseExtractor/model/en-pos-maxent.bin";
	private POSModel model;
	private POSTaggerME tagger;
	
	public Postagger(){
		System.out.println("Create an instance of class 'Postagger'.");
		
		String tags[] = null;
		InputStream modelIn = null;
		try {
			System.out.println("Loading model...");
			modelIn = new FileInputStream(model_path);
			model = new POSModel(modelIn);
			POSTaggerME tagger = new POSTaggerME(model);
			System.out.println("Model loaded success!");
		}
		catch (IOException e) {
		  // Model loading failed, handle the error
		  e.printStackTrace();
		  System.out.println("Error in class 'Postagger'.");
		  System.exit(1);
		}
		finally {
		  if (modelIn != null) {
		    try {
		      modelIn.close();
		    }
		    catch (IOException e) {
		    	System.out.println("Error in class 'Postagger'.");
				  System.exit(1);
		    }
		  }
		  
		}
	}
	
	public String [] run(String [] sent){
		String tags[] = null;
	
		tags = tagger.tag(sent);
		return tags;
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println("Testing in the postagger program.");
		
		String sent[] = new String[]{"Most", "large", "cities", "in", "the", "US", "had",
                "morning", "and", "afternoon", "newspapers", "."};	
		
		Postagger postagger = new Postagger();
		
		String tags[] = postagger.run(sent);
		for (String tag: tags){
			System.out.println(tag);
		}
	}
}
