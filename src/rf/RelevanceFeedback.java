package rf;

import java.util.ArrayList;
import java.util.List;

import prediction.Predictor;
import clustering.ClusterMain;
import clustering.DataPreprocessor;
import parser.SPAQLQueryParser;
import weka.core.Instances;

public class RelevanceFeedback {

	// to do - add in a config file
	String [] predAttribName = {"WorkflowDurationFromPlatform","QueryTotalResponseTimeFromClient","DeciderTotalExecutionTime","TransformerTotalExecutionTime","IdentifierTotalExecutionTime","SelecterTotalExecutionTime","ReasonerTotalExecutionTime","WorkflowPluginNb","DeciderThreadsStartedNB","WorkflowDuration"};
	String [] predAttribIdx = {"11","12","14","15","16","17","18","19","20","21"};
	int noPredAttr = (predAttribName.length==predAttribIdx.length)?predAttribIdx.length:(-1);
	
	ClusterMain cm = null;
	List<List<Predictor>> pAttrib = new ArrayList<List<Predictor>>();	

	public static void main(String[] args) {

		RelevanceFeedback d = new RelevanceFeedback();
		
		d.initClusteringPrediction();
		
		d.predictAttrForQuery("PREFIX foaf: <http://xmlns.com/foaf/0.1/> SELECT ?name1 WHERE { ?person1 foaf:knows ?person2 . ?person1 foaf:name  ?name1 . ?person2 foaf:name  \"Ionel Giosan\" .   } ORDER BY ?name1 ");
		d.predictAttrForQuery("PREFIX owl: <http://www.w3.org/2002/07/owl#> PREFIX xsd: <http://www.w3.org/2001/XMLSchema#> PREFIX rdfs: <http://www.w3.org/2000/01/rdf-schema#> PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> PREFIX foaf: <http://xmlns.com/foaf/0.1/> PREFIX dc: <http://purl.org/dc/elements/1.1/> PREFIX dbres: <http://dbpedia.org/resource/> PREFIX dbpedia2: <http://dbpedia.org/property/> PREFIX dbpedia: <http://dbpedia.org/> PREFIX skos: <http://www.w3.org/2004/02/skos/core#>  SELECT ?name WHERE {   ?monument skos:broader <http://dbpedia.org/resource/Category:Visitor_attractions_in_Cluj> .   ?monument rdfs:label ?name . } ");
		d.predictAttrForQuery("PREFIX foaf: <http://xmlns.com/foaf/0.1/> SELECT  ?weblog ?sha1 ?name WHERE { ?x foaf:knows ?y . ?y foaf:name  \"Ionel Chopin\". ?x foaf:name ?name . ?x foaf:weblog ?weblog. ?x foaf:mbox_sha1sum ?sha1 } ");
		
	}

	void initClusteringPrediction()
	{
		/* ******************** */
		/* 	TRAINING MODULE		*/
		/* ******************** */
		
		/* STEP 1: PREPROCESS THE ARFF FILE, AND GENERATE UNIQUE ATTRIBUTES FOR NAMESPACES */

		DataPreprocessor dp = new DataPreprocessor();
		dp.loadInitialData("arff/metricsMethod.arff");
		dp.tokenizeAttribute("QueryNamespaceValues","[,] ", "ns_");
		DataPreprocessor.outputArffInstances(dp.getTransformedData(),"arff/metricsMethod_ns.arff");
		
		/* STEP 2: PERFORM CLUSTERING BASED ONLY ON THE ATTRIBUTES OF THE QUERY */	
		String attributesToRemoveIndices = "1, 2, 4, 11-21"; //for the file metricsMethod.arff
		try {			
			cm = new ClusterMain("arff/metricsMethod_ns.arff", attributesToRemoveIndices); 
			cm.buildClusterEM();
			cm.saveData();
		} catch (Exception ex) {
			System.out.println(ex.getMessage());
		}		
				
		/* STEP 3: PERFORM LINEAR REGRESSION FOR EACH CLUSTER */

		//build the predicted attributes string list
		
		for (int k=0; k<noPredAttr; k++)
		{
			List<Instances> aInstances = cm.getClusteredInstancesList();
			List<Predictor> pCluster = new ArrayList<Predictor>();
			for (int i=0; i<aInstances.size(); i++)
				pCluster.add(new Predictor());
			
			for (int i=0; i<pCluster.size(); i++)
			{
				pCluster.get(i).setTrainingInstances(aInstances.get(i));
				pCluster.get(i).setConsideredAttributes("3,5-10,"+predAttribIdx[k]+",22-last");
				pCluster.get(i).setClassAttribute(predAttribName[k]);
				pCluster.get(i).buildRegressionModel();
				
				/*
				for (int j=0;j<pCluster.get(i).getCoefficients().length;j++)
					System.out.print(pCluster.get(i).getCoefficients()[j]+" ");
				System.out.println();
				
				DataPreprocessor.outputArffInstances(pCluster.get(i).getFilteredData(),"arff/outpred"+i+".arff");
				*/
			}
			pAttrib.add(pCluster);
		}
	}
	
	String predictAttrForQuery(String query)
	{
		/* ******************** */
		/* 	TEST MODULE			*/
		/* ******************** */		

		
		/* STEP 1: TEST - GIVEN A QUERY GENERATE THE ARFF CORRESPONDING TO IT; */
		//Given a Query perform the parsing of it an try to predict the cluster to which it belongs
		
		String QueryContent1 = query;
		SPAQLQueryParser parser = new SPAQLQueryParser();			
		parser.parseQuery(QueryContent1);
		parser.generateArff("arff/outQuery.arff", true);		
	
		/* STEP 2: MANUALLY SPLIT THE NAMESPACE VALUES !!! */
		/* was done by the parser */
		
		/* STEP 3: FIND THE CLUSTER TO WHICH THE ARFF / INSTANCE CORRESPONDING TO THE QUERY BELONGS */
	
		String attributesToRemoveFromQuery = "1, 2, 4, 5"; //for the file metricsMethod.arff
		String classAttributeToRemoveFromQuery = "11"; //for the file metricsMethod.arff
		
		double clusterIndex [] = cm.getClusterIdForQuery("arff/outQuery.arff", attributesToRemoveFromQuery,classAttributeToRemoveFromQuery,"arff/testPredQuery.arff");
		System.out.print("\n The query \n\t" + QueryContent1+ "\n\t belongs to cluster ");
		for (int i = 0; i < clusterIndex.length; i++)
			System.out.print(" " + clusterIndex[i]);

		/* STEP 4: GIVEN THE CLUSTER INDEX PERFORM LINEAR REGRESSION TO PREDICT THE OTHER PARAMETERS OF THE QUERY */
		/* STEP 4: predicted parameters are: 
		 *  WorkflowDurationFromPlatform 
		 *  QueryTotalResponseTimeFromClient
		 * 	QueryCompletionStatus	
		 * 	DeciderTotalExecutionTime		
		 * 	TransformerTotalExecutionTime	
		 * 	IdentifierTotalExecutionTime		
		 * 	SelecterTotalExecutionTime		
		 * 	ReasonerTotalExecutionTime		
		 *  WorkflowPluginNb			
		 *  DeciderThreadsStartedNB		
		 *  WorkflowDuration	
		 *  */
	
		String outputResult="";
		
		for (int k=0; k<noPredAttr; k++)
		{
			outputResult+="Predicted "+predAttribName[k]+" : "+pAttrib.get(k).get((int)(clusterIndex[0])).predictTestInstance("arff/testPredQuery.arff")+"\n";
		}

		Instances iCluster=cm.getClusteredInstancesList().get((int)clusterIndex[0]);
		int numSuccessful=0, numFail=0;
		for (int j=0; j<iCluster.numInstances();j++)
		{
			String vStatus=iCluster.instance(j).stringValue(iCluster.attribute("QueryCompletionStatus"));
			if (vStatus.equals("SuccessfulQuery")) numSuccessful++;
			if (vStatus.equals("FailedQuery")) numFail++;
		}
		outputResult+="Predicted "+"QueryCompletionStatus successful probability"+" : "+ ((double)numSuccessful/(numSuccessful+numFail))+"\n";
		System.out.println();
		System.out.println(outputResult);
		return outputResult;
	}
	
}
