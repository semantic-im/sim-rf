#pragma once

#include "../include/Variant.h"

class Feature {
	CString FeatureName;
	CString FeatureType;
	libcv::variant FeatureValue;
	std::vector<CString> categoryValues; //for features of type category, we store an array of possible categories (classes): eg: SuccessfulQuery, FailedQuery, Pedestrian, NonPedestrian

	enum featureTypes {
		ft_date,
		ft_string,
		ft_numeric,
		ft_category
	};
public:
	void addCategoryValue(CString val){
		CString crt;
		for (int i = 0; i < (int)this->categoryValues.size(); i++)
		{
			crt = this->categoryValues.at(i);
			if (crt.MakeLower() == val.MakeLower()) // the category value exists already in the array, so we don't add it anymore
				return;
		}
		this->categoryValues.push_back(val);
	}
	const CString& getFeatureName()const {
		return this->FeatureName;
	}

	void setFeatureName(const CString& fn){
		this->FeatureName = fn;
	}

	const CString getFeatureType() const{
		return this->FeatureType;
	}

	void setFeatureType(const CString &fn){
		this->FeatureType = fn;
	}

	libcv::variant getFeatureValue()const{
		return this->FeatureValue;
	}

	void setFeatureValue(libcv::variant fv){
		this->FeatureValue = fv;
	}

	Feature(void){
		this->categoryValues.clear();
	}
	Feature(const CString &fn, const CString &ft, libcv::variant fv ){
		this->FeatureName = fn;
		this->FeatureType = ft;
		this->FeatureValue = fv;
	}

	~Feature(void){
		this->categoryValues.clear();
	}

};