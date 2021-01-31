#include "GD_ClientApp.h"
#include "GD_Entity.h"
#include "Utils.h"

#include <InputEvent.hpp>

using namespace godot;

void GD_Entity::_register_methods()
{
	register_method("_ready", &GD_Entity::_ready);
	register_method("_process", &GD_Entity::_process);
	register_method("_physics_process", &GD_Entity::_physics_process);
	register_method("_input", &GD_Entity::_input);
}

GD_Entity::GD_Entity()
{
	m_id = -1;
	m_pClientApp = NULL;
	setValid(false);
}

GD_Entity::~GD_Entity()
{
}

void GD_Entity::_init()
{
	Godot::print("GD_Entity::Init");
}

void GD_Entity::_ready()
{
	Godot::print("GD_Entity::_ready");
}

void GD_Entity::_process(float _delta)
{
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("GD_Entity::_process");
}

void GD_Entity::_physics_process(float _delta)
{
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("GD_Entity::_physics_process");
}

void GD_Entity::_input(const Ref<InputEvent> event)
{
	//Godot::print("GD_Entity::_input: " + event->as_text());
}

bool GD_Entity::initEntity(int id, Node* pClientApp)
{
	m_id = id;
	m_pClientApp = pClientApp;

	setValid(true);

	char buffer[16];
	String entityName = getEntityName();
	String nodeName;
	
	if (isPlayer())
		nodeName = GD_CLIENTAPP_PLAYER_ENTITY_NODE;
	else
	{
		nodeName = GD_CLIENTAPP_OTHER_ENTITY_NODE;
		nodeName = nodeName + "_" + _itoa(id, buffer, 10);
	}
	
	set_name(nodeName);

	return true;
}

bool GD_Entity::destroyEntity(void)
{
	setValid(false);
	return true;
}

int GD_Entity::get_id(bool bIgnoreValid)
{
	if (bIgnoreValid)
		return m_id;
	else
		return isValid() ? m_id : -1;
}

String GD_Entity::getEntityName()
{
	if (!isValid())
		return "";

	if (m_entityName != "")
		return m_entityName;
	
	GD_ClientApp* pApp = (GD_ClientApp*)m_pClientApp;
	bool bPlayer;
	KBEntity* kbentity = pApp->getEntityById(m_id, bPlayer);
	if (kbentity)
		m_entityName =  kbentity->getName();
	
	//char buffer[16]; _itoa((int)m_id, buffer, 10);
	//String s = "getEntityName (";	s = s + buffer + "): " + kbentity->getName();
	//Godot::print(s);

	return m_entityName;
}
