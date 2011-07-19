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
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import weka.core.Attribute;
import weka.core.Instance;
import weka.core.Instances;

public class BestConfigurator implements Serializable{

    //SELECT ?s ?p ?o WHERE { { ?s ?p ?o . ?s ?p "trauma"}  UNION {?s ?p ?o . ?s ?p "yellow fever"} UNION {?s ?p ?o . ?s ?p "APRAXIAS" } UNION {?s ?p ?o . ?s ?p "level;" } UNION {?s ?p ?o . ?s ?p "superoxide" } UNION {?s ?p ?o . ?s ?p "envenomations" } }

    DataPreprocessor dp = null;
    ClusterMain cm = null;
    ClusterMain cWorkflow = null;

    int attribWorkflowNo;
    ArrayList<HashSet<String>> configurations = new ArrayList<HashSet<String>>();
    ArrayList<HashMap<String,Double>> evalConfigurationsMetric= new ArrayList<HashMap<String, Double>>();
    ArrayList<String> bestConfigurations = new ArrayList<String>();

    String [] predAttribName = {"QueryTotalResponseTime","WorkflowTotalResponseTime"};
    String [] predAttribIdx = {"14","28"};
    int noPredAttr = (predAttribName.length==predAttribIdx.length)?predAttribIdx.length:(-1);
    List<List<List<Predictor>>> allPredictors; //list indexes: queryCluster, predictedAttribute, workflowConfig

    boolean isTrained = false;

    public void setDataPreprocessor(DataPreprocessor dp)
    {
        this.dp=dp;
    }

    public void initialClustering(String attributesToRemoveIndices, int noClusters, int attribWorkflowNo)
    {
        dp.resetInitialData();
        try {
                cm = new ClusterMain(dp.getTransformedData(), attributesToRemoveIndices);
                cm.buildClusterEM(noClusters);
//                cm.saveData("D:/ddd");

        } catch (Exception ex) {
                System.out.println(ex.getMessage());
        }

        this.attribWorkflowNo=attribWorkflowNo;
        buildWorkflowsIdsSet();
    }


    public void secondClusteringOnWorkflowInsideCluster(int clusterNo, String attributesToRemoveIndices)
    {
        if (configurations.get(clusterNo).size()>0)
        {
            Instances subset=cm.getClusteredInstancesList().get(clusterNo);
            try {
                    cWorkflow = new ClusterMain(subset, attributesToRemoveIndices);
                    cWorkflow.buildClusterEM(configurations.get(clusterNo).size());
//                    cWorkflow.saveData("D:/inside"+clusterNo);
            } catch (Exception ex) {
                    System.out.println(ex.getMessage());
            }
        }
        else
            cWorkflow=null;
    }

    public List<List<Predictor>> buildPredictorsInsideCluster(int clusterNo)
    {
        if (cWorkflow!=null)
        {
            List<List<Predictor>> pAttrib = new ArrayList<List<Predictor>>();

            for (int k=0; k<noPredAttr; k++)
            {
                    List<Instances> aInstances = cWorkflow.getClusteredInstancesList();
                    List<Predictor> pClusterConfig = new ArrayList<Predictor>();
                    for (int i=0; i<aInstances.size(); i++)
                    {
                            if (aInstances.get(i).numInstances()>0)
                            {
                                pClusterConfig.add(new Predictor());
                                pClusterConfig.get(i).setDetails(aInstances.get(i).instance(0).stringValue(attribWorkflowNo));
                                pClusterConfig.get(i).setTrainingInstances(aInstances.get(i));
                                pClusterConfig.get(i).setConsideredAttributes("5-12,"+predAttribIdx[k]);
                                pClusterConfig.get(i).setClassAttribute(predAttribName[k]);
                                pClusterConfig.get(i).buildRegressionModel();
                            }
                            else
                                pClusterConfig.add(null);
                    }
                    pAttrib.add(pClusterConfig);
            }
            return pAttrib;
        }
        else
            return null;
    }

    public void buildAllPredictors(String attributesToRemoveIndices)
    {
       allPredictors = new ArrayList<List<List<Predictor>>>();
        for (int i=0;i<cm.noClusters;i++)
        {
            System.out.println("--->>> Clusteri de configuratii inauntrul clusterului "+i);
            secondClusteringOnWorkflowInsideCluster(i, attributesToRemoveIndices);
            List<List<Predictor>> currentPred=  buildPredictorsInsideCluster(i);
            allPredictors.add(currentPred);
        }
    }


    public String predictQuery(Instances qInst, int instanceNo, String attributesToRemoveFromQuery)
    {
        int clusterNo=cm.getClusterIdForQuery(qInst, instanceNo, attributesToRemoveFromQuery);
        //String outputResult="***Prediction***\nTest instance is in the cluster: "+clusterNo+"\n"+"Best configuration: "+bestConfigurations.get(clusterNo)+"\n";
        String outputResult="Best configuration workflow: "+bestConfigurations.get(clusterNo)+"\n";
        for (int k=0; k<noPredAttr; k++)
        {
            List<Predictor> lp=allPredictors.get(clusterNo).get(k);
            Predictor applyPredictor=null;
            for(int t=0;t<lp.size();t++)
                if (lp.get(t)!=null && lp.get(t).getDetails().equals(bestConfigurations.get(clusterNo)))
                {
                    applyPredictor=lp.get(t);
                    break;
                }

            Instances filteredAttr=DataPreprocessor.selectSubsetAttributes(qInst, applyPredictor.consideredAttributes);
            
            double value=applyPredictor.predictTestInstance(filteredAttr.instance(instanceNo));
            outputResult+="Predicted "+predAttribName[k]+": "+value+"\n";
        }
        return outputResult;
    }

    public String predictQuery(String query, String attributesToRemoveFromQuery)
    {
        Instances qInst = new Instances (dp.getTransformedData(), 0);

        Instance t = new Instance(dp.getTransformedData().numAttributes());
        for (int a=5-1;a<=12-1;a++)
            t.setValue(a, 0);

        Attribute a_QuerySizeInCharacters=qInst.attribute("QuerySizeInCharacters");
        t.setValue(a_QuerySizeInCharacters, query.length());

        qInst.add(t);

//        DataPreprocessor.outputArffInstances(qInst, "D:/test");

        return predictQuery(qInst, 0, attributesToRemoveFromQuery);
    }


    public void buildWorkflowsIdsSet()
    {
        for (int k=0;k<cm.noClusters;k++)
        {
            Instances iCluster=cm.getClusteredInstancesList().get(k);

            HashSet<String> currentConfigs=new HashSet<String>();
        
            for (int i=0; i<iCluster.numInstances();i++)
            {
                String currentValue=iCluster.instance(i).stringValue(attribWorkflowNo);
                currentConfigs.add(currentValue);
            }

            configurations.add(k, currentConfigs);

            System.out.println("\nConfigurations in cluster " + k +": ");
            Iterator it=currentConfigs.iterator();
            while (it.hasNext()){
                System.out.print(it.next() + " ");
            }
             System.out.println("\n");
        }
    }

    public int findClusterInstance(Instances instancesSet, int instanceNo, String attributesToRemoveFromQuery)
    {
        return cm.getClusterIdForQuery(instancesSet, instanceNo, attributesToRemoveFromQuery);
    }


    public double evalMetric(Instance i)
    {
        return i.value(13);
    }

    public String computeClusterBestConfiguration(int clusterNo)
    {
        System.out.println("In clusterul: "+clusterNo);

        HashMap<String,Double> evalClusterConfigurationsMetric=new HashMap<String,Double>();
        int i;

        Iterator it=configurations.get(clusterNo).iterator();
        while (it.hasNext()){
            ArrayList<Integer> vCluster=cm.getClusterInstancesIdxs(clusterNo);
            String clusterWorkflowValue=it.next().toString();
            double metricValueConfig=0;
            int noConsideredValues=0;
            for (int j=0;j<vCluster.size();j++)
            {
                String attribWorkflowValue=dp.getTransformedData().instance(vCluster.get(j)).stringValue(attribWorkflowNo);
                if (attribWorkflowValue.equals(clusterWorkflowValue))
                {
                    metricValueConfig+=evalMetric(dp.getTransformedData().instance(vCluster.get(j)));
                    noConsideredValues++;
                }
            }
            metricValueConfig/=noConsideredValues;
            System.out.println("\tConfiguratia: "+clusterWorkflowValue+" metrica medie evaluata: "+metricValueConfig);
            
            evalClusterConfigurationsMetric.put(clusterWorkflowValue, metricValueConfig);
        }

        evalConfigurationsMetric.add(clusterNo, evalClusterConfigurationsMetric);

        String result="";
        Double min=Double.MAX_VALUE;
        Set s=evalClusterConfigurationsMetric.entrySet();
        it=s.iterator();
        while(it.hasNext())
        {
            Map.Entry m =(Map.Entry)it.next();

            String key=(String)m.getKey();
            Double value=(Double)m.getValue();

            if (value<min)
            {
                value=min;
                result=key;
            }
        }
        return result;
    }


    public void computeAllClusterBestConfigurations()
    {
        bestConfigurations=new ArrayList<String>();
        for (int i=0;i<cm.noClusters;i++)
        {
            String bestConfig=computeClusterBestConfiguration(i);
            bestConfigurations.add(i,bestConfig);
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

    public static BestConfigurator loadModel(String fileName)
    {
      try {
              File file = new File(fileName);
              ObjectInputStream in = new ObjectInputStream(new FileInputStream(file));
              BestConfigurator r = (BestConfigurator) in.readObject();
              in.close();
              return r;
        } catch (ClassNotFoundException ex) {
               return null;
        } catch (IOException e) {
                return null;
        }
    }
 }
