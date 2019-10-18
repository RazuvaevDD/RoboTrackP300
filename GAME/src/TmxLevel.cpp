#include "TmxLevel.h"

#include <iostream>
#include "../lib/tinyxml2/tinyxml2.h"

using namespace tinyxml2;

// On Windows, backslash '\\' is directory separator in paths
// On UNIX paths separated with slash '/'. (MacOS - ?)
#if defined(_WIN32)
const char DIR_SEPARATOR = '\\';
#else
const char DIR_SEPARATOR = '/';
#endif

// Возвращает родительский каталог с указанным путем
std::string GetParentDirectory(const std::string &filepath)
{
    const size_t slashPos = filepath.find_last_of('/');
    std::string parentDir;
    if (slashPos != std::string::npos)
    {
        parentDir = filepath.substr(0, slashPos);
    }
    return parentDir;
}

// Объединяет два фрагмента пути: путь к каталогу и имя файла
std::string JoinPaths(const std::string &path, const std::string &subpath)
{
    if (path.empty())
    {
        return subpath;
    }
    if (subpath.empty())
    {
        return path;
    }

    return path + DIR_SEPARATOR + subpath;
}

// Парсит RGB в шестнадцатеричном формате, например "6d9fb9"
sf::Color ParseColor(const std::string &hexRGB)
{
    char *pEnd = nullptr;
    const long hexValue = strtol(hexRGB.c_str(), &pEnd, 16);
    if (*pEnd != '\0')
    {
        throw std::runtime_error(hexRGB + " is not valid hex-encoded RGB color");
    }

    const uint8_t red = uint8_t((hexValue >> 16) % 256);
    const uint8_t green = uint8_t((hexValue >> 8) % 256);
    const uint8_t blue = uint8_t(hexValue % 256);

    return sf::Color(red, green, blue);
}

float ParseFloat(const std::string &str)
{
    char *pEnd = nullptr;
    const float value = strtof(str.c_str(), &pEnd);
    if (*pEnd != '\0')
    {
        throw std::runtime_error("'" + str + "' is not a float number");
    }

    return value;
}

int TmxObject::GetPropertyInt(const std::string &propertyName)
{
    return std::stoi(properties[propertyName].c_str());
}

float TmxObject::GetPropertyFloat(const std::string &propertyName)
{
    return ParseFloat(properties[propertyName].c_str());
}

std::string TmxObject::GetPropertyString(const std::string &propertyName)
{
    return properties[propertyName];
}

void TmxObject::MoveBy(const sf::Vector2f &movement)
{
    rect.left += movement.x;
    rect.top += movement.y;
    sprite.move(movement);
}

void TmxObject::MoveTo(const sf::Vector2f &position)
{
    rect.left = position.x;
    rect.top = position.y;
    sprite.setPosition(position);
}

bool TmxLevel::LoadFromFile(const std::string &filepath)
{
    XMLDocument levelFile;

    // Загружаем XML в XMLDocument
    if (levelFile.LoadFile(filepath.c_str()) != XML_SUCCESS)
    {
        throw std::runtime_error("Loading level \"" + filepath + "\" failed.");
    }

    // Элемент <map> должен быть корневым в TMX формате.
    XMLElement *map = levelFile.FirstChildElement("map");
    if (map == 0)
    {
        throw std::runtime_error("<map> element not found");
    }

    // Пример Map элемента:
    //   <map version="1.0" orientation="orthogonal"
    //    width="10" height="10" tilewidth="34" tileheight="34">
    m_width = std::stoi(map->Attribute("width"));
    m_height = std::stoi(map->Attribute("height"));
    m_tileWidth = std::stoi(map->Attribute("tilewidth"));
    m_tileHeight = std::stoi(map->Attribute("tileheight"));

    // Получить описание набора плиток и GID первой плитки (Идентификатор группы).
    XMLElement *tilesetElement = map->FirstChildElement("tileset");
    m_firstTileID = std::stoi(tilesetElement->Attribute("firstgid"));

    // <image> содержит текстуру набора плиток
    XMLElement *image = tilesetElement->FirstChildElement("image");
    const std::string imageFilename = image->Attribute("source");
    const std::string imagePath = JoinPaths(GetParentDirectory(filepath), imageFilename);

    sf::Color matteColor = sf::Color(0, 0, 0, 0);
    if (image->Attribute("trans") != nullptr)
    {
        matteColor = ParseColor(image->Attribute("trans"));
    }

    sf::Image img;
    if (!img.loadFromFile(imagePath))
    {
        std::cout << "Failed to load tile sheet." << std::endl;
        return false;
    }

    // Устанавливаем "мозаичный" цвет мозаики, используемый для создания прозрачного изображения на фоне, заполненном матовым цветом.
    img.createMaskFromColor(matteColor);
    // Загружаем текстуру из файла.
    m_tilesetImage.loadFromImage(img);
    // Сохраняем резкость текстуры при изменении размера.
    m_tilesetImage.setSmooth(false);

    // Получаем количество строк и столбцов в плитке.
    const int columns = m_tilesetImage.getSize().x / m_tileWidth;
    const int rows = m_tilesetImage.getSize().y / m_tileHeight;

    // Собираем текстуры в список
    // (Каждый текстурный прямоугольник представляет собой подизображение в изображении набора плиток)
    std::vector<sf::IntRect> subRects;
    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < columns; x++)
        {
            sf::IntRect rect;
            rect.top = y * m_tileHeight;
            rect.height = m_tileHeight;
            rect.left = x * m_tileWidth;
            rect.width = m_tileWidth;
            subRects.push_back(rect);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    /// Парсим слои плиток
    ///
    XMLElement *layerElement = map->FirstChildElement("layer");
    while (layerElement)
    {
        TmxLayer layer;

        // Скопируем прозрачность, если она установлена ​​в XML, или сделаем слой непрозрачным (255).
        if (layerElement->Attribute("opacity") != nullptr)
        {
            float opacity = ParseFloat(layerElement->Attribute("opacity"));
            layer.opacity = sf::Uint8(255 * opacity);
        }
        else
        {
            layer.opacity = 255;
        }

        // <data> содержит описание нескольких плиток.
        XMLElement *layerDataElement = layerElement->FirstChildElement("data");
        if (layerDataElement == nullptr)
        {
            std::cout << "Bad map. No layer information found." << std::endl;
        }

        // <tile> содержит описание одной плитки.
        XMLElement *tileElement = layerDataElement->FirstChildElement("tile");
        if (tileElement == nullptr)
        {
            std::cout << "Bad map. No tile information found." << std::endl;
            return false;
        }

        int x = 0;
        int y = 0;
        while (tileElement)
        {
            const int tileGID = std::stoi(tileElement->Attribute("gid"));
            const int subRectToUse = tileGID - m_firstTileID;

            // Определяем текстуру прямоугольника для каждой плитки.
            if (subRectToUse >= 0)
            {
                sf::Sprite sprite;
                sprite.setTexture(m_tilesetImage);
                sprite.setTextureRect(subRects[subRectToUse]);
                sprite.setPosition(static_cast<float>(x * m_tileWidth), static_cast<float>(y * m_tileHeight));
                sprite.setColor(sf::Color(255, 255, 255, layer.opacity));

                layer.tiles.push_back(sprite);
            }

            tileElement = tileElement->NextSiblingElement("tile");

            x++;
            if (x >= m_width)
            {
                x = 0;
                y++;
                if (y >= m_height)
                {
                    y = 0;
                }
            }
        }

        m_layers.push_back(layer);

        layerElement = layerElement->NextSiblingElement("layer");
    }

    ////////////////////////////////////////////////////////////////////////////
    /// Сбор списка объектов.
    ///

    // Если есть слои объектов.
    if (map->FirstChildElement("objectgroup") != nullptr)
    {
        XMLElement *objectGroupElement = map->FirstChildElement("objectgroup");
        while (objectGroupElement)
        {
            // Войдем в узел <object>
            XMLElement *objectElement;
            objectElement = objectGroupElement->FirstChildElement("object");

            while (objectElement)
            {
                // Сбор свойств объекта - тип, имя, позиция и т.д.
                std::string objectType;
                if (objectElement->Attribute("type") != nullptr)
                {
                    objectType = objectElement->Attribute("type");
                }
                std::string objectName;
                if (objectElement->Attribute("name") != nullptr)
                {
                    objectName = objectElement->Attribute("name");
                }
                float x = std::stof(objectElement->Attribute("x"));
                float y = std::stof(objectElement->Attribute("y"));
                float width = 0;
                float height = 0;

                sf::Sprite sprite;
                sprite.setTexture(m_tilesetImage);
                sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
                sprite.setPosition(x, y);

                if (objectElement->Attribute("width") != nullptr)
                {
                    width = std::stof(objectElement->Attribute("width"));
                    height = std::stof(objectElement->Attribute("height"));
                }
                else
                {
                    const size_t index = std::stoi(objectElement->Attribute("gid")) - m_firstTileID;
                    width = static_cast<float>(subRects[index].width);
                    height = static_cast<float>(subRects[index].height);
                    sprite.setTextureRect(subRects[index]);
                    sprite.setOrigin(0, height);
                }

                // Определяем объект
                TmxObject object;
                object.name = objectName;
                object.type = objectType;
                object.sprite = sprite;

                sf::FloatRect objectRect;
                objectRect.top = y;
                objectRect.left = x;
                objectRect.height = height;
                objectRect.width = width;
                object.rect = objectRect;

                // Читаем свойства объекта
                XMLElement *properties = objectElement->FirstChildElement("properties");
                if (properties != nullptr)
                {
                    XMLElement *prop = properties->FirstChildElement("property");
                    if (prop != nullptr)
                    {
                        while (prop)
                        {
                            std::string propertyName = prop->Attribute("name");
                            std::string propertyValue = prop->Attribute("value");

                            object.properties[propertyName] = propertyValue;

                            prop = prop->NextSiblingElement("property");
                        }
                    }
                }

                // Добавляем объект в список
                m_objects.push_back(object);

                objectElement = objectElement->NextSiblingElement("object");
            }
            objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
        }
    }
    else
    {
        std::cout << "No object layers found..." << std::endl;
    }

    return true;
}

TmxObject TmxLevel::GetFirstObject(const std::string &name) const
{
    // Только первый объект с заданным именем
    for (size_t i = 0; i < m_objects.size(); i++)
        if (m_objects[i].name == name)
            return m_objects[i];
    throw std::runtime_error("Object with name " + name + " was not found");
}

std::vector<TmxObject> TmxLevel::GetAllObjects(const std::string &name) const
{
    // Все объекты с указанным именем
    std::vector<TmxObject> vec;
    for (size_t i = 0; i < m_objects.size(); i++)
    {
        if (m_objects[i].name == name)
        {
            vec.push_back(m_objects[i]);
        }
    }

    return vec;
}

sf::Vector2i TmxLevel::GetTileSize() const
{
    return sf::Vector2i(m_tileWidth, m_tileHeight);
}

float TmxLevel::GetTilemapWidth() const
{
    return static_cast<float>(m_tileWidth * m_width);
}

float TmxLevel::GetTilemapHeight() const
{
    return static_cast<float>(m_tileHeight * m_height);
}

sf::Vector2f TmxLevel::GetTilemapSize() const
{
    return sf::Vector2f(GetTilemapWidth(), GetTilemapHeight());
}

void TmxLevel::Draw(sf::RenderTarget &target) const
{
    const sf::FloatRect viewportRect = target.getView().getViewport();

    // Рисовать все плитки без отрисовки объектов
    for (const auto &layer : m_layers)
    {
        for (const auto &tile : layer.tiles)
        {
            if (viewportRect.intersects(tile.getLocalBounds()))
            {
                target.draw(tile);
            }
        }
    }
}
