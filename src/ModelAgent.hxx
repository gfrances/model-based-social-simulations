
#ifndef __ModelAgent_hxx__
#define __ModelAgent_hxx__

#include <Agent.hxx>
#include <string>

namespace Model
{

class ModelAgent : public Engine::Agent
{
	int _resources; // MpiBasicAttribute

    void move();
    void collect();
    void consume();
    void checkStarvation();
public:
	ModelAgent( const std::string & id );
	virtual ~ModelAgent();

    // decision making process occurs inside this method
	void selectActions();
    // update state after selecting and executing actions
	void updateState();

	void registerAttributes();
	void serialize();

	void setResources( int resources );
	int getResources() const;

	////////////////////////////////////////////////
	// This code has been automatically generated //
	/////// Please do not modify it ////////////////
	////////////////////////////////////////////////
	ModelAgent( void * );
	void * fillPackage();
	void sendVectorAttributes(int);
	void receiveVectorAttributes(int);
	////////////////////////////////////////////////
	//////// End of generated code /////////////////
	////////////////////////////////////////////////

};

} // namespace Model

#endif // __ModelAgent_hxx__

