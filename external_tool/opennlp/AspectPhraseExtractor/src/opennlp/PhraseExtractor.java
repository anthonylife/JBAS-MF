package opennlp;

public class PhraseExtractor {

	/**
	 * This class convert raw input text into tagged text data.
	 * 
	 * @author Wei Zhang
	 * @date 2013/10/9
	 */
	private Chunker chunker;
	private Postagger postagger;
	
	public PhraseExtractor(){
		chunker = new Chunker();
		postagger = new Postagger();
		//System.out.println("Model loaded success!");
	}
	
	public void tagging(String paths[]){
		
	}
	
	public static void main(String[] args) {
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
		
		
	}

}
