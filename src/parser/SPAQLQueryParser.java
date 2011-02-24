package parser;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Collection;
import java.util.Date;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.hp.hpl.jena.query.Query;
import com.hp.hpl.jena.query.QueryException;
import com.hp.hpl.jena.query.QueryExecution;
import com.hp.hpl.jena.query.QueryExecutionFactory;
import com.hp.hpl.jena.query.QueryFactory;
import com.hp.hpl.jena.query.SortCondition;
import com.hp.hpl.jena.rdf.model.Model;
import com.hp.hpl.jena.rdf.model.ModelFactory;
import com.hp.hpl.jena.shared.PrefixMapping;
import com.hp.hpl.jena.sparql.core.Var;
import com.hp.hpl.jena.sparql.syntax.Element;

import weka.core.Attribute;
import weka.core.FastVector;
import weka.core.Instance;
import weka.core.Instances;
import weka.core.converters.ArffSaver;

public class SPAQLQueryParser {
	
	private String namespaces[] = {"http://dbpedia.org/", "http://dbpedia.org/class/yago/", "http://dbpedia.org/property/", "http://dbpedia.org/resource/", "http://dbpedia.org/resource/category:", "http://purl.org/dc/elements/1.1/", "http://www.w3.org/1999/02/22-rdf-syntax-ns#", "http://www.w3.org/2000/01/rdf-schema#", "http://www.w3.org/2004/02/skos/core#", "http://www.w3.org/people/berners-lee/card#", "http://xmlns.com/foaf/0.1/" };
	
	private String QueryContent;
	
	private int QuerySizeInBytes;
	
	private int QueryNamespaceNb;	
	
	//for namespaces we will store for the moment the keys and the values in the PREFIX list; 
	// for example PREFIX foaf: <http://xmlns.com/foaf/0.1/> will result in QueryNamespaceKeys = foaf and 
	// QueryNamespaceValues = http://xmlns.com/foaf/0.1/
	private Set<String> QueryNamespaceKeys; 
	private Set<String> QueryNamespaceValues;
	
	private int QueryVariablesNb;
	
	private int QueryDataSetSourcesNb;
	private List<String> QueryDataSetSources;

	private int QueryOperatorsNb;
	
	private int QueryResultOrderingNb;
	
	private int QueryResultLimitNb;
	
	private int QueryResultOffsetNb;
	
	private int QuerySizeInTriples;
	
	public SPAQLQueryParser(){
		this.QuerySizeInBytes = 0;
		this.QuerySizeInTriples = 0;	
	}

	/**
	 * @return the queryContent
	 */
	public String getQueryContent() {
		return QueryContent;
	}

	/**
	 * @param queryContent the queryContent to set
	 */
	public void setQueryContent(String queryContent) {
		QueryContent = queryContent;
	}

	/**
	 * @return the querySizeInBytes
	 */
	public int getQuerySizeInBytes() {
		return QuerySizeInBytes;
	}

	/**
	 * @param querySizeInBytes the querySizeInBytes to set
	 */
	public void setQuerySizeInBytes(int querySizeInBytes) {
		QuerySizeInBytes = querySizeInBytes;
	}

	/**
	 * @return the queryNamespaceNb
	 */
	public int getQueryNamespaceNb() {
		return QueryNamespaceNb;
	}

	/**
	 * @param queryNamespaceNb the queryNamespaceNb to set
	 */
	public void setQueryNamespaceNb(int queryNamespaceNb) {
		QueryNamespaceNb = queryNamespaceNb;
	}

	/**
	 * @return the queryVariablesNb
	 */
	public int getQueryVariablesNb() {
		return QueryVariablesNb;
	}

	/**
	 * @param queryVariablesNb the queryVariablesNb to set
	 */
	public void setQueryVariablesNb(int queryVariablesNb) {
		QueryVariablesNb = queryVariablesNb;
	}

	/**
	 * @return the queryDataSetSourcesNb
	 */
	public int getQueryDataSetSourcesNb() {
		return QueryDataSetSourcesNb;
	}

	/**
	 * @param queryDataSetSourcesNb the queryDataSetSourcesNb to set
	 */
	public void setQueryDataSetSourcesNb(int queryDataSetSourcesNb) {
		QueryDataSetSourcesNb = queryDataSetSourcesNb;
	}

	/**
	 * @return the queryDataSetSources
	 */
	public List<String> getQueryDataSetSources() {
		return QueryDataSetSources;
	}

	/**
	 * @param queryDataSetSources the queryDataSetSources to set
	 */
	public void setQueryDataSetSources(List<String> queryDataSetSources) {
		QueryDataSetSources = queryDataSetSources;
	}

	/**
	 * @return the queryOperatorsNb
	 */
	public int getQueryOperatorsNb() {
		return QueryOperatorsNb;
	}

	/**
	 * @param queryOperatorsNb the queryOperatorsNb to set
	 */
	public void setQueryOperatorsNb(int queryOperatorsNb) {
		QueryOperatorsNb = queryOperatorsNb;
	}

	/**
	 * @return the queryResultOrderingNb
	 */
	public int getQueryResultOrderingNb() {
		return QueryResultOrderingNb;
	}

	/**
	 * @param queryResultOrderingNb the queryResultOrderingNb to set
	 */
	public void setQueryResultOrderingNb(int queryResultOrderingNb) {
		QueryResultOrderingNb = queryResultOrderingNb;
	}

	/**
	 * @return the queryResultLimitNb
	 */
	public int getQueryResultLimitNb() {
		return QueryResultLimitNb;
	}

	/**
	 * @param queryResultLimitNb the queryResultLimitNb to set
	 */
	public void setQueryResultLimitNb(int queryResultLimitNb) {
		QueryResultLimitNb = queryResultLimitNb;
	}

	/**
	 * @return the queryResultOffsetNb
	 */
	public int getQueryResultOffsetNb() {
		return QueryResultOffsetNb;
	}

	/**
	 * @param queryResultOffsetNb the queryResultOffsetNb to set
	 */
	public void setQueryResultOffsetNb(int queryResultOffsetNb) {
		QueryResultOffsetNb = queryResultOffsetNb;
	}
	
	public int parseQuery(String QueryContent){
		//parses the QueryContent and fills up all the fields in this class
		
		this.setQueryContent(QueryContent);
		try {
			Query query = QueryFactory.create(QueryContent);
			parsePrefixInformation(query);
			
			parseVariablesInformation(query);
			
			parseDataSetSourcesInformation(query);
			
			parseOrderByStatement(query);
			
			parseLimitStatement(query);
			
			parseOffsetStatement(query);
			
			getModel(query);
		} catch (QueryException qe) {
			System.out.println("Query creation returned an exception " + qe.getMessage());
			return -1;
		}
		catch (Exception ex){
			System.out.println("Query creation returned an exception " + ex.getMessage());
			return -1;
		}
		
		return 0;				
	}
	
	public void parsePrefixInformation(Query query) throws Exception{
		// gets the information related to the prefixes, i.e QueryDataSetSourcesNb and QueryDataSetSources
		
		//display the number of prefix statements along with the namespaces they represent
		PrefixMapping prefixMapping = query.getPrefixMapping();
		
		Map<String,String> prefixEquivalents = prefixMapping.getNsPrefixMap();
		
		System.out.println("I have "+ prefixEquivalents.size() + " prefixes and these are:");
		
		//get only the prefixes that are used in the query; 
		
		Set <String> keySetTmp = prefixEquivalents.keySet();
		Collection <String> entrySetTmp = prefixEquivalents.values();
		
		//find what keys are used in the query; 
		// some prefixes may appear in the prefix declaration but they are not effectively used in the corpus of the query
		//hence further parsing is needed to the the exact list of prefixes;
		
		if (!query.isSelectType()) {
			System.out.println("ONLY WORKS FOR SELECT QUERIES");
			throw new Exception("NOT A SELECT Query!");			
		}				
		String QueryBody = query.toString().toLowerCase();		
	
		int selectIndex = QueryBody.indexOf("select");
		if (selectIndex < 0){
			System.out.println("ONLY WORKS FOR SELECT QUERIES");
			throw new Exception("NOT A SELECT Query!");			
		}
	
		String selectBody = QueryBody.substring(selectIndex); 
		Iterator<String> it = keySetTmp.iterator();
		int keyValueIndex = -1;
		Set<String> keys = new HashSet();
		Set<String> values = new HashSet();
		
		while (it.hasNext()) {
			// Return the value to which this map maps the specified key.
			String crtKey = it.next();
			String crtValue = prefixEquivalents.get(crtKey);			
			System.out.println(crtKey + " - " + crtValue);
			keyValueIndex = selectBody.indexOf(crtKey);
			if ( keyValueIndex > 0) {
				keys.add(crtKey);
				values.add(crtValue);
			}				
			else {
				keyValueIndex = selectBody.indexOf(crtValue);
				if ( keyValueIndex > 0) {
					keys.add(crtKey);
					values.add(crtValue);
				}
			}
		}
		this.setQueryNamespaceNb(keys.size());
		this.setQueryNamespaceKeys(keys);
		this.setQueryNamespaceValues(values);
		System.out.println("\n-----------------------------------------------------------\n");		
	}
	
	public void parseVariablesInformation(Query query){
		//finds all the information related to variables in the query
		
		/*
		/*display the number of variables contained in the query and the variables
		*/
		Element queryBlock = query.getQueryPattern();
		
		Set<String> variablesSet = new HashSet<String>();
		
		//save the variables from where clause - they might not contain all the variables from select
		Set<Var> variablesInWhereClause = queryBlock.varsMentioned();
		
		//save the variables from where clause to the list containing all the variables
		Iterator<Var> its =  variablesInWhereClause.iterator();
		while (its.hasNext())
			variablesSet.add(its.next().toString().substring(1));
		
		//save the variables from select clause to the list containing all the variables
		for (int i = 0; i < query.getResultVars().size(); i++)
			variablesSet.add(query.getResultVars().get(i));
		
		System.out.println("I have " + variablesSet.size() + " variables and these are:");
		Iterator<String> itss =  variablesSet.iterator();
		while (itss.hasNext())
			System.out.println(itss.next());
		
		System.out.println("\n-----------------------------------------------------------\n");
		
		this.setQueryVariablesNb(variablesSet.size());

	}
	
	public void parseDataSetSourcesInformation(Query query){
		//extract the from clauses
		
		/*
		/*display the number of FROM sources contained in the query and the sources
		*/
		
		if (query.hasDatasetDescription()) {
			List<String> graphUris = query.getGraphURIs();
		
			System.out.println("I have " + graphUris.size() + " data sources and these are:");
			for (int i = 0; i < graphUris.size(); i++)
				System.out.println(graphUris.get(i));
		
			System.out.println("\n-----------------------------------------------------------\n");

			this.setQueryDataSetSourcesNb(graphUris.size());
			this.setQueryDataSetSources(graphUris);
		}
	}
	public void parseOrderByStatement (Query query){
		
		/*
		/*display the number of fields in the ORDER BY statement and the fields
		*/
		
		if (query.hasOrderBy()) {
			List<SortCondition> orderByFields =  query.getOrderBy();
			System.out.println("I have " + orderByFields.size() + " fields in my ORDER BY clause and these are:");
		
			for (int i = 0; i < orderByFields.size(); i++)
				System.out.println(orderByFields.get(i));
		
			System.out.println("\n-----------------------------------------------------------\n");
		
			this.setQueryResultOrderingNb(orderByFields.size());
		}
	}
	
	public void parseLimitStatement (Query query){
		/*
		/*display the value of n from the LIMIT n statement
		*/
		if (query.hasLimit()) {
				long limit = query.getLimit();
				System.out.println("The value of n from my LIMIT n statement is: "+limit);
		
				System.out.println("\n-----------------------------------------------------------\n");
		
				this.setQueryResultLimitNb((int)limit);
		}
	}

	public void parseOffsetStatement (Query query){
		/*
		*display the value of m from the OFFSET m statement
		*/
		if (query.hasOffset()) {
			long offset = query.getOffset();
			System.out.println("The value of m from my OFFSET m statement is: "+offset);
		
			System.out.println("\n-----------------------------------------------------------\n");

			this.setQueryResultOffsetNb((int) offset);
		}
	}
	
	public void getModel (Query query){
		Model model = ModelFactory.createDefaultModel() ;
		QueryExecution qExec = QueryExecutionFactory.create(query, model) ;
		
		System.out.println( "Size of the model " + model.size() + " QueryExecution " + qExec.toString());
	}

	/**
	 * @return the querySizeInTriples
	 */
	public int getQuerySizeInTriples() {
		return QuerySizeInTriples;
	}

	/**
	 * @param querySizeInTriples the querySizeInTriples to set
	 */
	public void setQuerySizeInTriples(int querySizeInTriples) {
		QuerySizeInTriples = querySizeInTriples;
	}

	/**
	 * @return the queryNamespaceKeys
	 */
	public Set<String> getQueryNamespaceKeys() {
		return QueryNamespaceKeys;
	}

	/**
	 * @param queryNamespaceKeys the queryNamespaceKeys to set
	 */
	public void setQueryNamespaceKeys(Set<String> queryNamespaceKeys) {
		QueryNamespaceKeys = queryNamespaceKeys;
	}

	/**
	 * @return the queryNamespaceValues
	 */
	public Set<String> getQueryNamespaceValues() {
		return QueryNamespaceValues;
	}

	/**
	 * @param queryNamespaceValues the queryNamespaceValues to set
	 */
	public void setQueryNamespaceValues(Set<String> queryNamespaceValues) {
		QueryNamespaceValues = queryNamespaceValues;
	}
	
	public Instances generateArff (String arff_file_name, boolean save) {
		//creates an Instance corresponding to the fields stored in the parser
		// if save is true then the instance is saved in the arff_file_name
		
		/* step 1: set-up the attributes */
		FastVector attributes = new FastVector();
		
		//add the attributes to the vector
		
		//attribute 1: QueryTimestamp of type date
		attributes.addElement(new Attribute("QueryTimestamp","yyyy-MM-dd HH:mm:ss"));
		
		//attribute 2: QueryContent of type string
		attributes.addElement(new Attribute("QueryContent",(FastVector)null));
		
		//attribute 3: QueryNamespaceNb type NUMERIC
		attributes.addElement(new Attribute("QueryNamespaceNb"));
		
		//attribute 4: QueryNamespaceKeys of type STRING
		attributes.addElement(new Attribute("QueryNamespaceKeys", (FastVector)null));
		
		//attribute 5: QueryNamespaceValues of type STRING - this attribute is not added anymore because we will parse it and add an attribute for each namespace
		// if a certain namespace in the query is not present in the namespaces[] list, it will be ignored
		//IDEA- probably we could add an attribute that tells us how many namespaces have been ignored ? :D
		
		attributes.addElement(new Attribute("QueryNamespaceValues", (FastVector)null));
		
		//attribute 6: QueryVariablesNb of type NUMERIC
		attributes.addElement(new Attribute("QueryVariablesNb"));
		
		//attribute 7: QueryDataSetSourcesNb of type NUMERIC
		attributes.addElement(new Attribute("QueryDataSetSourcesNb"));
		
		//attribute 8: QueryResultOrderingNb of type NUMERIC
		attributes.addElement(new Attribute("QueryResultOrderingNb"));
		
		//attribute 9: QueryResultLimitNb of type NUMERIC
		attributes.addElement(new Attribute("QueryResultLimitNb"));
		
		//attribute 10: QueryResultOffsetNb of type NUMERIC
		attributes.addElement(new Attribute("QueryResultOffsetNb"));
		
		//attribute 11: QuerySizeInTriples of type NUMERIC
		attributes.addElement(new Attribute("QuerySizeInTriples"));
		
		//attribute 11: QuerySizeInTriples of type NUMERIC
		attributes.addElement(new Attribute("class"));
		
		
		// attribute 12-to 23 will be the query namespaces
		for (int i = 0; i < namespaces.length; i++){
			attributes.addElement(new Attribute("ns_"+namespaces[i]));
		}
				 
		//step 2: create Instances object
		Instances queryInstances = new Instances ("QueryInstancesRelation", attributes, 0);
		
		// step3: fill with data
		
		double vals[] = new double [queryInstances.numAttributes()];
		int attributeIndex = 0;
		
		try {
			//attribute 1: QueryTimestamp of type date
			DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
			Date date = new Date();		
			vals[attributeIndex] = queryInstances.attribute(attributeIndex++).parseDate(dateFormat.format(date));
		
			//attribute 2: QueryContent of type string
			vals[attributeIndex] = queryInstances.attribute(attributeIndex++).addStringValue(this.QueryContent);
		
			//attribute 3: QueryNamespaceNb type NUMERIC
			vals[attributeIndex++] = this.getQueryNamespaceNb();
		
			//attribute 4: QueryNamespaceKeys of type STRING
			vals[attributeIndex] = queryInstances.attribute(attributeIndex++).addStringValue(this.getQueryNamespaceKeys().toString());
		
			//attribute 5: QueryNamespaceValues of type STRING - elliminated because we add the splitted namespaces 
			vals[attributeIndex] = queryInstances.attribute(attributeIndex++).addStringValue(this.getQueryNamespaceValues().toString());
		
			//attribute 6: QueryVariablesNb of type NUMERIC
			vals[attributeIndex++] = this.getQueryVariablesNb();
		
			//attribute 7: QueryDataSetSourcesNb of type NUMERIC
			vals[attributeIndex++] = this.getQueryDataSetSourcesNb();
		
			//attribute 8: QueryResultOrderingNb of type NUMERIC
			vals[attributeIndex++] = this.getQueryResultOrderingNb();
		
			//attribute 9: QueryResultLimitNb of type NUMERIC
			vals[attributeIndex++] = this.getQueryResultLimitNb();
		
			//attribute 10: QueryResultOffsetNb of type NUMERIC
			vals[attributeIndex++] = this.getQueryResultOffsetNb();
		
			//attribute 11: QuerySizeInTriples of type NUMERIC
			vals[attributeIndex++] = this.getQuerySizeInTriples();
			
			//attribute 11: class -  of type NUMERIC
			vals[attributeIndex++] = 0;
			
			
			for(int i = 0; i < namespaces.length; i++){
				if (this.QueryNamespaceValues.contains(namespaces[i]))
					vals[attributeIndex++] = 1;
				else
					vals[attributeIndex++] = 0;
			}			
			queryInstances.add(new Instance(1.0, vals));
			
			if(save) {
				//save queryInstances to the arff_file_name
				ArffSaver saver = new ArffSaver();
				FileOutputStream fs = new FileOutputStream(arff_file_name);
				saver.setInstances(queryInstances);
				saver.setDestination(fs);				
				saver.writeBatch();
				fs.close();
			}

		} catch (ParseException pe){
			System.out.println(pe.getMessage());
		} catch (IOException ioe) {
			System.out.println(ioe.getMessage());
		}				
		return queryInstances;
	}
}