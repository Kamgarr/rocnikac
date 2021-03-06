#include "Loader.h"
#include <fstream>
#include <sstream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

gg::MLoader::MLoader(irr::IrrlichtDevice *device) : m_irrDevice(device)
{
}

std::vector<std::unique_ptr<gg::MObject>> gg::MLoader::load(std::string level)
{
    m_objects = std::vector<std::unique_ptr<gg::MObject>>();
    m_objectCreator.reset(new MObjectCreator(m_irrDevice));

    std::string current_line;
    //open level file
    std::fstream fin;
    fin.open(level, std::fstream::in);

    //skybox
    std::getline(fin, current_line);
    if(!loadSkybox(split(std::stringstream(current_line))))
    {
        std::cerr << "Loading skybox failed!\n";
    }

    //ground
    std::getline(fin, current_line);
    std::unique_ptr<MObject> object(m_objectCreator->createSolidGround(split(std::stringstream(current_line))));
    if(object)
    {
        m_objects.push_back(std::move(object));
    }

    //ship
    std::getline(fin, current_line);
    std::unique_ptr<MObject> obj(m_objectCreator->createBoxedRigidBody(split(std::stringstream(current_line))));
    if(obj)
    {
        m_objects.push_back(std::move(obj));
    }

    //buildings
    while(std::getline(fin, current_line))
    {
        if(current_line != "")
        {
            std::unique_ptr<MObject> obj(m_objectCreator->createMeshRigidBody(split(std::stringstream(current_line))));
            if(obj)
            {
                m_objects.push_back(std::move(obj));
            }
        }
    }
    fin.close();
    return std::move(m_objects);
}

bool gg::MLoader::loadSkybox(std::vector<std::string> &&files)
{
    if(files.size() < 6)
    {
        return false;
    }

    IVideoDriver *irrDriver = m_irrDevice->getVideoDriver();

    irrDriver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
    m_irrDevice->getSceneManager()->addSkyBoxSceneNode(irrDriver->getTexture((m_media + files[0]).c_str()),
                                                       irrDriver->getTexture((m_media + files[1]).c_str()),
                                                       irrDriver->getTexture((m_media + files[2]).c_str()),
                                                       irrDriver->getTexture((m_media + files[3]).c_str()),
                                                       irrDriver->getTexture((m_media + files[4]).c_str()),
                                                       irrDriver->getTexture((m_media + files[5]).c_str()));
    irrDriver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

    return true;
}

std::vector<std::string> gg::MLoader::split(std::stringstream &&input)
{
    std::vector<std::string> parts;
    for(std::string item; std::getline(input, item, ';'); parts.push_back(item))
    {
    }
    return std::move(parts);
}
