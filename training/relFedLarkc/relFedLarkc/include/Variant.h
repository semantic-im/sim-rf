//Copyright 2007 Silviu Bota
#ifndef VARIANT_H_INCLUDED
#define VARIANT_H_INCLUDED

#include <bitset>
#include <string>
#include <sstream>
#include <hash_map>

namespace libcv {
	
	class VariantTree;

	/** class to hold different types, based on an internal flag*/
	class variant {
	public:
		enum types {
			type_null,
			type_bool,
			type_int,
			type_float,
			type_double,
			type_string,
			type_binary,
			type_cpvoid,
			type_sublist,
		};
	private:
		 types t;
		union {
			bool val_bool;
			int val_int;
			float val_float;
			double *val_double;
			std::string *val_string;
			std::vector<char> *val_binary;
			const void *val_cpvoid;
			VariantTree *val_sublist;
		};
	public:
		class variant_error {};

		variant():t(type_null){}
		variant(bool v):t(type_bool){val_bool = v;}
		variant(int v):t(type_int) {val_int = v;}
		variant(float v):t(type_float) {val_float = v;}
		variant(double v):t(type_double) {val_double = new double(v);}
		variant(const std::string &v):t(type_string) {val_string = new std::string(v);}
		variant(const void* v):t(type_cpvoid), val_cpvoid(v) {}
		variant(const char *v, size_t n):t(type_binary) {
			val_binary = new std::vector<char>(n);
			for(std::vector<char>::size_type i=0;i<n;i++) {
				(*val_binary)[i] = v[i];
			}
		}
		static variant variant::from_string(types type, const char* str);
		

		inline variant(const VariantTree& v);
		inline variant(const variant &v);
		inline ~variant();
		inline variant& operator=(const variant& v);

		bool get_bool() const {if(t != type_bool)throw variant_error();else return val_bool;}
		int get_int() const {if(t != type_int)throw variant_error();else return val_int;}
		float get_float() const {if(t != type_float)throw variant_error();else return val_float;}
		double get_double() const {if(t != type_double)throw variant_error();else return *val_double;}
		const std::string *get_string() const {if(t != type_string)throw variant_error();else return val_string;}
		const void *get_cpvoid() const {if(t != type_cpvoid)throw variant_error();else return val_cpvoid;}
		const std::vector<char> *get_binary() const {if(t != type_binary)throw variant_error();else return val_binary;}
		const VariantTree *get_sublist() const {if(t != type_sublist)throw variant_error(); else return val_sublist;}
		VariantTree *get_sublist() {if(t != type_sublist)throw variant_error(); else return val_sublist;}



		types get_type()  const {return t;}
		static const char* get_type_name(types t);
		static types type_from_name(const char* name);
		bool is_null() const {return t==type_null;}
		bool is_bool() const {return t==type_bool;}
		bool is_int() const {return t==type_int;}
		bool is_float() const {return t==type_float;}
		bool is_double() const {return t==type_double;}
		bool is_string() const {return t==type_string;}
		bool is_cpvoid() const {return t==type_cpvoid;}
		bool is_binary() const {return t==type_binary;}
		bool is_sublist() const {return t==type_sublist;}

		bool toBool() const {
			switch(t) {
			case type_bool:
				return val_bool;
			case type_int:
				return val_int!=0;
			case type_float:
				return val_float!=0;
			case type_double:
				return val_double!=0;
			default:
				throw variant_error();
			}
		}

		int toInt() const {
			switch(t) {
			case type_bool:
				return val_bool?1:0;
			case type_int:
				return val_int;
			case type_float:
				return int(val_float+.5f);
			case type_double:
				return int(*val_double+.5);
			default:
				throw variant_error();
			}

		}
		float toFloat() const {
			switch(t) {
			case type_bool:
				return val_bool?1.f:0.f;
			case type_int:
				return float(val_int);
			case type_float:
				return val_float;
			case type_double:
				return float(*val_double);
			default:
				throw variant_error();
			}
		}

		double toDouble() const {
			switch(t) {
			case type_bool:
				return val_bool?1.0:0.0;
			case type_int:
				return double(val_int);
			case type_float:
				return double(val_float);
			case type_double:
				return double(*val_double);
			default:
				throw variant_error();
			}
		}

		std::string toString() const {
			std::ostringstream str;
			str<<*this;
			return str.str();
		}

		friend std::ostream& operator<<(std::ostream& o, const variant& v) {
			switch(v.t) {
				case type_null:o<<"null";break;
				case type_bool:o<<v.val_bool;break;
				case type_int:o<<v.val_int;break;
				case type_float:o<<v.val_float;break;
				case type_double:o<<*v.val_double;break;
				case type_string:o<<*v.val_string;break;
				case type_cpvoid:o<<v.val_cpvoid;break;
				case type_binary: {
					for(std::vector<char>::size_type i = 0;i<v.val_binary->size();i++)
						o<<std::hex<<unsigned(i);break;
					}
				default: throw variant_error();
			}
			return o;
		}

	
	};

	void SplitStringIntoParts(const std::string &key, char delimiter, std::list<std::string> &outp, int max_parts=1000000);

	class VariantTree {
	
	private:
		struct val{
			val(variant v, std::list<std::string>::iterator p) :v(v), p(p) {}
			variant v;
			std::list<std::string>::iterator p;
		};
		typedef stdext::hash_map<std::string, val> mtype;
		mtype m;
		typedef std::list<std::string> ltype;
		ltype l;

	public:
		friend std::ostream& operator<<(std::ostream& o, const VariantTree& vm);

		template<class T> void pushBackMultiPath(T itstart, T itend, const variant& val) {
			if(itstart==itend)return;
			T itplus = itstart;
			itplus++;
			VariantTree *m = this;
			for(;itplus!=itend;itstart++, itplus++) {
				VariantTree::const_iterator it = m->find(*itstart);
				if(it == m->end() || !m->get(*itstart)->is_sublist()) {
					m->push_back(*itstart, variant(VariantTree()));
				}
				m=m->get(*itstart)->get_sublist();
			}
			m->push_back(*itstart,val);
		}
		template<class T> void pushBackMultiPath(const std::string &path, char delimiter, const variant& val) {
			std::list<std::string> lst;
			SplitStringIntoParts(path, delimiter, lst);
			pushBackMultiPath(lst.begin(), lst.end(), val);

		}

		/*const variant operator[](const std::string& k) const {
			mtype::const_iterator it = m.find(k);
			if(it==m.end())return variant();
			else return it->second.v;
		}*/

		variant* get(const std::string& k)  {
			mtype::iterator it = m.find(k);
			if(it==m.end())return 0;
			else return &it->second.v;
		}

		const variant* get(const std::string& k)  const{
			mtype::const_iterator it = m.find(k);
			if(it==m.end())return 0;
			else return &it->second.v;
		}

		bool getBool(const std::string& k) const {
			return get(k)->get_bool();
		}

		bool toBool(const std::string& k) const {
			return get(k)->toBool();
		}

		int getInt(const std::string& k) const {
			return get(k)->get_int();
		}

		int toInt(const std::string& k) const {
			return get(k)->toInt();
		}

		float getFloat(const std::string& k) const {
			return get(k)->get_float();
		}

		float toFloat(const std::string& k) const {
			return get(k)->toFloat();
		}

		double getDouble(const std::string& k) const {
			return get(k)->get_double();
		}

		double toDouble(const std::string& k) const {
			return get(k)->toDouble();
		}

		const std::string* getString(const std::string& k) const {
			return get(k)->get_string();
		}

		std::string toString(const std::string& k) const {
			return get(k)->toString();
		}


		template <class T>
		variant* get(T beg, T end)  {
			T beg2 = beg;
			beg2++;
			VariantTree *vt = this;
			for(;beg2!=end;beg++, beg2++) {
				variant *v = vt->get(*beg);
				if(!v->is_sublist())return 0;
				vt = v->get_sublist();
			}
			return vt->get(*beg);
		}

		template <class T>
		const variant* get(T beg, T end) const {
			T beg2 = beg;
			beg2++;
			const VariantTree *vt = this;
			for(;beg2!=end;beg++, beg2++) {
				const variant *v = vt->get(*beg);
				if(!v->is_sublist())return 0;
				vt = v->get_sublist();
			}
			return vt->get(*beg);
		}

		variant* getMultiPath(const std::string& path, char delimiter)  {
			std::list<std::string> lst;
			SplitStringIntoParts(path, delimiter, lst);
			return get(lst.begin(), lst.end());
		}

		const variant* getMultiPath(const std::string &path, char delimiter) const {
			std::list<std::string> lst;
			SplitStringIntoParts(path, delimiter, lst);
			return get(lst.begin(), lst.end());
		}
	

		bool push_front(const std::string &k, variant v) {
			mtype::iterator it = m.find(k);
			if(it!=m.end()) {
				it->second = val(v ,it->second.p); //no insertion, just change value
				return false;
			}
			l.push_front(k);//add both to list and map
			m.insert(mtype::value_type(k, val(v, l.begin())));
			return true;
		}
		
		bool push_back(const std::string &k, variant v) {
			mtype::iterator it = m.find(k);
			if(it!=m.end()) {
				it->second= val(v ,it->second.p); //no insertion, just change value
				return false;
			}
			l.push_back(k);//add both to list and map
			ltype::iterator last = l.end();
			last--;
			m.insert(mtype::value_type(k, val(v, last)));
			return true;
		}

		bool remove(const std::string &k) {
			mtype::iterator it = m.find(k);
			if(it==m.end())return false; //nothing to remove
			l.erase(it->second.p);
			m.erase(it);
			return true;
		}

		void clear() {
			m.clear();
			l.clear();
		}

		typedef ltype::iterator iterator;
		typedef ltype::const_iterator const_iterator;
		
		iterator begin() {
			return l.begin();
		}

		const_iterator begin() const {
			return l.begin();
		}

		iterator end() {
			return l.end();
		}

		const_iterator end() const {
			return l.end();
		}

		const_iterator find(const std::string& s) const {
			mtype::const_iterator it = m.find(s);
			if(it==m.end())return l.end();
			return it->second.p;
		}
		void serialize(std::ostream &s) const;
		static bool deserialize(std::istream &s, VariantTree &vt);

	private:
		void IndentedPrint(std::ostream& os, int indent=0) const;


	};
	variant::variant(const VariantTree& v):t(type_sublist) {val_sublist = new VariantTree(v);}
	
	variant::variant(const variant &v) {
			t = v.t;
			switch(t) {
				case type_null:break;
				case type_int:val_int = v.val_int;break;
				case type_float:val_float = v.val_float;break;
				case type_double: val_double = new double(*v.val_double);break;
				case type_string: val_string = new std::string(*v.val_string);break;
				case type_cpvoid: val_cpvoid = v.val_cpvoid;break;
			case type_binary: val_binary = new std::vector<char>(*v.val_binary);break;
			case type_sublist: val_sublist = new VariantTree(*v.val_sublist);break;
				default: throw variant_error();
			}
		}


	variant::~variant() {
			switch(t) {
				case type_null:break;
				case type_int:break;
				case type_float:break;
				case type_double:delete val_double;break;
				case type_string:delete val_string;break;
				case type_binary:delete val_binary;break;
			case type_sublist:delete val_sublist;break;
				default: throw variant_error();
			}
		}

	variant& variant::operator=(const variant& v)  {
			if(this == &v) return *this;
			switch(t) {
				case type_null:break;
				case type_int:break;
				case type_float:break;
				case type_double:delete val_double;break;
				case type_string:delete val_string;break;
				case type_binary:delete val_binary;break;
			case type_sublist:delete val_sublist;break;
				default: throw variant_error();
			}
			t=v.t;
			switch(t) {
				case type_null:break;
				case type_int:val_int = v.val_int;break;
				case type_float:val_float = v.val_float;break;
				case type_double:val_double = new double(*v.val_double);break;
				case type_string:val_string = new std::string(*v.val_string);break;
			case type_binary:val_binary = new std::vector<char>(*v.val_binary);break;
			case type_sublist:val_sublist = new VariantTree(*v.val_sublist);break;
				default: throw variant_error();
			}
			return *this;
		}

	void WriteVariantTreeToScaborStyleStream(const VariantTree& vm, std::ostream &stream, const std::list<std::string> &prefix=std::list<std::string>());
	void WriteVariantTreeToRegStyleStream(const VariantTree& vm, std::ostream &stream, const std::list<std::string> &prefix=std::list<std::string>());
	void WriteVariantTreeToXML(const VariantTree& vm, std::ostream &stream);

	void ReadVariantTreeFromScaborStyleStream(VariantTree& vm, std::istream &stream, int levels=0, const std::list<std::string>& prefix=std::list<std::string>());
	void ReadVariantTreeFromRegStyleStream(VariantTree& vm, std::istream &stream, const std::list<std::string>& prefix=std::list<std::string>());
	
	variant VariantFromScaborStyleString(const std::string & str);

	
}



#endif
