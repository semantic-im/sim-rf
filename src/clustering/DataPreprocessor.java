package clustering;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import weka.core.Attribute;
import weka.core.Instances;
import weka.core.converters.ArffLoader;
import weka.core.converters.ArffSaver;
import weka.core.tokenizers.WordTokenizer;
import weka.filters.unsupervised.attribute.StringToWordVector;

public class DataPreprocessor {
	private Instances initialData;
	private Instances transformedData;
	
	public void loadInitialData(String fileName)
	{
		 ArffLoader loader = new ArffLoader();
		 try {
			loader.setFile(new File(fileName));
			initialData = loader.getDataSet();
			initialData.setClassIndex(-1);
			transformedData=new Instances(initialData);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void setInitialData(Instances initialData)
	{
		this.initialData = initialData;
		this.transformedData=new Instances(initialData);
	}	
	
	public Instances getTransformedData()
	{
		return transformedData;
	}	
	
	public void tokenizeAttribute(String attrName, String delimiters, String prefix)
	{
		Attribute a = transformedData.attribute(attrName);
		tokenizeAttribute(a.index()+1, delimiters, prefix);
	}
	
	public void tokenizeAttribute(int idx, String delimiters, String prefix)
	{
		if (idx<=transformedData.numAttributes()-1)
		{
			int wordsToKeep = 1000;
			
			StringToWordVector filter = new StringToWordVector(wordsToKeep);
			
			WordTokenizer wT= new WordTokenizer();
			wT.setDelimiters(delimiters);
			filter.setTokenizer(wT);
			filter.setAttributeIndices(Integer.toString(idx));
			filter.setLowerCaseTokens(true);
			filter.setOutputWordCounts(false);
			filter.setAttributeNamePrefix(prefix);
			
			try {
				filter.setInputFormat(transformedData);
				transformedData = weka.filters.Filter.useFilter(transformedData,filter); 
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

	public void tokenizeAttributes(String[] attrNameList, String delimiters, String prefix)
	{
		String attrIndices="";
		for (int i=0; i<attrNameList.length; i++)
		{
			Attribute a = transformedData.attribute(attrNameList[i]);
			if (i!=0) attrIndices+=","; 
			attrIndices+=(a.index()+1);
		}
		tokenizeAttributes(attrIndices, delimiters, prefix);
	}
	
	public void tokenizeAttributes(String attrNoList, String delimiters, String prefix)
	{
		int wordsToKeep = 1000;
		
		StringToWordVector filter = new StringToWordVector(wordsToKeep);
		
		WordTokenizer wT= new WordTokenizer();
		wT.setDelimiters(delimiters);
		filter.setTokenizer(wT);
		filter.setAttributeIndices(attrNoList);
		filter.setLowerCaseTokens(true);
		filter.setOutputWordCounts(false);
		filter.setAttributeNamePrefix(prefix);
		
		try {
			filter.setInputFormat(transformedData);
			transformedData = weka.filters.Filter.useFilter(transformedData,filter); 
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public static void outputArffInstances(Instances in, String filename)
	{
	    ArffSaver saver = new ArffSaver();
	    saver.setInstances(in);
	    try {
	    	FileOutputStream fs = new FileOutputStream(filename);
	    	saver.setDestination(fs);
		    saver.writeBatch();
		    fs.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
