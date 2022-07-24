#include "SceneManager.h"

#include <Engine/Scene/EntityManager.hpp>
#include <SceneComponent.hpp>

#include <QList>
#include <ScanMaterial/ScanMaterial.hpp>

SceneManager::SceneManager(QObject* parent)
    : QObject { parent }
{
}

void SceneManager::init()
{
    assert(m_engine != nullptr);
    assert(m_viewer != nullptr);
    assert(m_sys != nullptr);
    assert(m_mdiArea != nullptr);
    assert(m_imageManipulator != nullptr);

    Ra::Engine::Data::ScanMaterial::registerMaterial();

    // Create and initialize entity and component
    Ra::Engine::Scene::Entity* e = m_engine->getEntityManager()->createEntity("Scene entity");
    SceneComponent* c = new SceneComponent(e);
    //    Dof6Component* c = new Dof6Component(e);
    m_sys->addComponent(e, c);
    c->initialize();

    m_initialized = true;

    m_viewer->prepareDisplay();

    m_sensorModel.setColumnCount(5);
    QStringList header;
    header << "Sensor name"
           << "Format"
           << "Dimension"
           << "Size"
           << "Frequency";
    m_sensorModel.setHorizontalHeaderLabels(header);
    //    QList<QStandardItem*> items;
    //    items.append(QStandardItem("hello"));
    //    items << "ouou";
    //    m_sensorModel.setItem(0, 0, new QStandardItem("root"));

    //    m_sensorModel.appendRow(&items);
    //    m_sensorModel.app

    //    m_sensorModel.se
    //    m_sensorModel.row
}

void SceneManager::delSensor(const std::string& sensorName)
{

    auto it = m_sensors.begin();
    int i = 0;
    while (it != m_sensors.end()) {
        auto& sensor = *it;
        if (sensor.m_inputStream->getSensorName() == sensorName) {
            std::cout << "[SceneManager] delSensor( " << sensorName << " )" << std::endl;


            it = m_sensors.erase(it);
            m_sensorModel.removeRow(i);
            continue;
        }
        ++it;
        ++i;
    }
}

const std::list<Sensor>& SceneManager::getSensors() const
{
    return m_sensors;
}

Sensor& SceneManager::getSensor(int iSensor)
{
    assert(iSensor < m_sensors.size());
    int i = 0;
    for (auto& sensor : m_sensors) {
        if (i == iSensor) {
            return sensor;
        }
        ++i;
    }
    assert(false);
    return m_sensors.front();
}

void SceneManager::attachSensorFromImageManipulator(int iSensor)
{
    for (auto & sensor : m_sensors) {
        sensor.detachFromImageManipulator();
    }

    int i = 0;
    for (auto& sensor : m_sensors) {
        if (i == iSensor) {
            sensor.attachFromImageManipulator();
        }
        ++i;
    }
}

void SceneManager::detachSensorFromImageManipulator(int iSensor)
{
    int i = 0;
    for (auto& sensor : m_sensors) {
        if (i == iSensor) {
            sensor.detachFromImageManipulator();
        }
        ++i;
    }
}
