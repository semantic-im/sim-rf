package rfjava;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutput;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

import weka.core.Instances;

public class RFClusteringRegression implements Serializable{
// PREFIX foaf: <http://xmlns.com/foaf/0.1/> SELECT ?name1 WHERE { ?person1 foaf:knows ?person2 . ?person1 foaf:name  ?name1 . ?person2 foaf:name  "Ionel Giosan" .   } ORDER BY ?name1

        String [] predAttribName = {"WorkflowDurationFromPlatform","QueryTotalResponseTimeFromClient","DeciderTotalExecutionTime","TransformerTotalExecutionTime","IdentifierTotalExecutionTime","SelecterTotalExecutionTime","ReasonerTotalExecutionTime","WorkflowPluginNb","DeciderThreadsStartedNB","WorkflowDuration"};
	String [] predAttribIdx = {"11","12","14","15","16","17","18","19","20","21"};
	int noPredAttr = (predAttribName.length==predAttribIdx.length)?predAttribIdx.length:(-1);
	DataPreprocessor dp = null;
        ClusterMain cm = null;
        boolean isTrained = false;
	List<List<Predictor>> pAttrib = new ArrayList<List<Predictor>>();	



/*	public static void main(String[] args) {

		RFClusteringRegression d = new RFClusteringRegression();
		
		d.initClusteringPrediction();
		
		d.predictAttrForQuery("PREFIX foaf: <http://xmlns.com/foaf/0.1/> SELECT ?name1 WHERE { ?person1 foaf:knows ?person2 . ?person1 foaf:name  ?name1 . ?person2 foaf:name  \"Ionel Giosan\" .   } ORDER BY ?name1 ");
		d.predictAttrForQuery("PREFIX owl: <http://www.w3.org/2002/07/owl#> PREFIX xsd: <http://www.w3.org/2001/XMLSchema#> PREFIX rdfs: <http://www.w3.org/2000/01/rdf-schema#> PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> PREFIX foaf: <http://xmlns.com/foaf/0.1/> PREFIX dc: <http://purl.org/dc/elements/1.1/> PREFIX dbres: <http://dbpedia.org/resource/> PREFIX dbpedia2: <http://dbpedia.org/property/> PREFIX dbpedia: <http://dbpedia.org/> PREFIX skos: <http://www.w3.org/2004/02/skos/core#>  SELECT ?name WHERE {   ?monument skos:broader <http://dbpedia.org/resource/Category:Visitor_attractions_in_Cluj> .   ?monument rdfs:label ?name . } ");
		d.predictAttrForQuery("PREFIX foaf: <http://xmlns.com/foaf/0.1/> SELECT  ?weblog ?sha1 ?name WHERE { ?x foaf:knows ?y . ?y foaf:name  \"Ionel Chopin\". ?x foaf:name ?name . ?x foaf:weblog ?weblog. ?x foaf:mbox_sha1sum ?sha1 }");
		
	}
*/

        public void setDataPreprocessor(DataPreprocessor dp)
        {
            this.dp=dp;
        }

	public void initClusteringPrediction() throws Exception
	{
                if (dp==null) return;

                /* ******************** */
		/* 	TRAINING MODULE		*/
		/* ******************** */

		/* STEP 1: PREPROCESS THE ARFF FILE, AND GENERATE UNIQUE ATTRIBUTES FOR NAMESPACES */

//		DataPreprocessor dp = new DataPreprocessor();
//		dp.loadInitialData(dataInputFile);
                dp.resetInitialData();
		dp.tokenizeAttribute("QueryNamespaceValues","[,] ", "ns_");
//		DataPreprocessor.outputArffInstances(dp.getTransformedData(),"arff/metricsMethod_ns.arff");
		
		/* STEP 2: PERFORM CLUSTERING BASED ONLY ON THE ATTRIBUTES OF THE QUERY */	
		String attributesToRemoveIndices = "1, 2, 4, 11-21"; //for the file metricsMethod.arff
		try {			
//			cm = new ClusterMain("arff/metricsMethod_ns.arff", attributesToRemoveIndices); 
			cm = new ClusterMain(dp.getTransformedData(), attributesToRemoveIndices);			
			cm.buildClusterEM(-1);
//			cm.saveData();
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

        public void saveModel(String fileName)
        {
          try {
                ObjectOutput out = new ObjectOutputStream(new FileOutputStream(fileName));
                out.writeObject(this);
                out.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        public static RFClusteringRegression loadModel(String fileName)
        {
          try {
                  File file = new File(fileName);
                  ObjectInputStream in = new ObjectInputStream(new FileInputStream(file));
                  RFClusteringRegression r = (RFClusteringRegression) in.readObject();
                  in.close();
                  return r;
            } catch (ClassNotFoundException ex) {
                   return null;
            } catch (IOException e) {
                    return null;
            }
        }


        public String predictAttrForQuery(String query)
	{
		/* ******************** */
		/* 	TEST MODULE			*/
		/* ******************** */		

		
		/* STEP 1: TEST - GIVEN A QUERY GENERATE THE ARFF CORRESPONDING TO IT; */
		//Given a Query perform the parsing of it an try to predict the cluster to which it belongs
		
		String QueryContent1 = query;
		SPARQLQueryParser parser = new SPARQLQueryParser();
		parser.parseQuery(QueryContent1);
//		parser.generateArff("arff/outQuery.arff", true);		
		Instances outInstances=parser.generateInstances();
	
		/* STEP 2: MANUALLY SPLIT THE NAMESPACE VALUES !!! */
		/* was done by the parser */
		
		/* STEP 3: FIND THE CLUSTER TO WHICH THE ARFF / INSTANCE CORRESPONDING TO THE QUERY BELONGS */
	
		String attributesToRemoveFromQuery = "1, 2, 4, 5"; //for the file metricsMethod.arff
		String classAttributeToRemoveFromQuery = "11"; //for the file metricsMethod.arff
		
		
//		double clusterIndex [] = cm.getClusterIdForQuery("arff/outQuery.arff", attributesToRemoveFromQuery,classAttributeToRemoveFromQuery,"arff/testPredQuery.arff");
		double clusterIndex [] = cm.getClusterIdForQuerys(outInstances, attributesToRemoveFromQuery,classAttributeToRemoveFromQuery);
		Instances testInstances = cm.getFilteredTestInstances();

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
			//double value=pAttrib.get(k).get((int)(clusterIndex[0])).predictTestInstance("arff/testPredQuery.arff");
			double value=pAttrib.get(k).get((int)(clusterIndex[0])).predictTestInstance(testInstances.firstInstance());
			outputResult+="Predicted "+predAttribName[k]+" : "+value+"\n";
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
