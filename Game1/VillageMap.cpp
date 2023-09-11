#include "stdafx.h"
#include "house.h"
#include "VillageMap.h"

VillageMap* VillageMap::Create(string name)
{
	VillageMap* temp = new VillageMap();
	temp->LoadFile("VillageMap.xml");
	temp->type = ObType::Actor;
	return temp;
}

VillageMap::VillageMap()
{
	cam = Camera::Create();
	cam->LoadFile("VillageCam.xml");
}

VillageMap::~VillageMap()
{
}

void VillageMap::Release()
{
	cam->SaveFile("VillageCam.xml");
	SaveFile("VillageMap.xml");
}

void VillageMap::Update()
{
	Actor::Update();
}

void VillageMap::Render()
{
	Actor::Render();

}

void VillageMap::Init()
{
}

void VillageMap::LateUpdate()
{
}

void VillageMap::Hierarchy()
{
	cam->RenderHierarchy();
	RenderHierarchy();
}

void VillageMap::ResizeScreen()
{
	cam->width = App.GetWidth();;
	cam->height = App.GetHeight();
	cam->viewport.x = 0;
	cam->viewport.y = 0;
	cam->viewport.width = App.GetWidth();
	cam->viewport.height = App.GetHeight();
}
