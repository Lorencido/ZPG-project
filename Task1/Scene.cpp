#include "Scene.h"

void Scene::add(const std::shared_ptr<DrawableObject>& obj) {
    obj->id = objects.size();
    objects.push_back(obj);
}

void Scene::remove(unsigned int id)
{
    if (id >= objects.size()) return;

    objects.erase(objects.begin() + id);

    for (unsigned int i = 0; i < objects.size(); i++)
        objects[i]->setID(i);

    if (selectedID == id) selectedID = -1;
    else if (selectedID > id) selectedID--;
}

void Scene::update(float time) {
    float dt = time;

    for (int i = 0; i < fireflies.size(); i++) {
        lights[i].position = fireflies[i]->light.position;
        fireflies[i]->update(dt);
    }

    for (auto& obj : objects)
        obj->update();
}

void Scene::draw() {
    if (skybox && camera)
        skybox->draw(*camera);

    for (auto& obj : objects) {
        glStencilFunc(GL_ALWAYS, obj->getID(), 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        obj->draw();
    }
}

void Scene::setSkybox(std::shared_ptr<SkyboxObject> sb)
{
    skybox = sb;
}

void Scene::setSelected(unsigned int id) {
    selectedID = int(id);

    std::cout << "Selected object ID = " << selectedID << std::endl;
}

int Scene::getSelectedID() const {
    return selectedID;
}
DrawableObject* Scene::getSelectedObject() {
    if (selectedID < 0 || selectedID >= objects.size()) return nullptr;
    return objects[selectedID].get();
}