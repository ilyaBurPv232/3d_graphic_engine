// #include "collisiondetector.h"
// #include <QVector3D>
// #include <cmath>
// #include <QDebug>

// QVector<CollisionInfo> CollisionDetector::detectCollisions(const QVector<Shape*>& bodies) {
//     QVector<CollisionInfo> collisions;

//     for (int i = 0; i < bodies.size(); ++i) {
//         Shape* a = bodies[i];
//         if (!a) continue;  // Пропускаем нулевые указатели

//         for (int j = i + 1; j < bodies.size(); ++j) {
//             Shape* b = bodies[j];
//             if (!b) continue;  // Пропускаем нулевые указатели

//             QVector3D minA, maxA, minB, maxB;
//             a->getAABB(minA, maxA);
//             b->getAABB(minB, maxB);

//             QVector3D collisionNormal;
//             float penetrationDepth;

//             if (aabbCollision(minA, maxA, minB, maxB, collisionNormal, penetrationDepth)) {
//                 CollisionInfo info;
//                 info.bodyA = a;
//                 info.bodyB = b;
//                 info.collisionNormal = collisionNormal;
//                 info.penetrationDepth = penetrationDepth;
//                 collisions.append(info);

//                 qDebug() << "Collision detected between shapes at positions:"
//                          << a->getPosition() << "and" << b->getPosition()
//                          << "Normal:" << collisionNormal
//                          << "Penetration:" << penetrationDepth;
//             }
//         }
//     }

//     return collisions;
// }

// bool CollisionDetector::aabbCollision(const QVector3D &minA, const QVector3D &maxA,
//                                       const QVector3D &minB, const QVector3D &maxB,
//                                       QVector3D& collisionNormal, float& penetrationDepth) {
//     // Проверка пересечения по осям
//     bool xOverlap = (minA.x() <= maxB.x()) && (maxA.x() >= minB.x());
//     bool yOverlap = (minA.y() <= maxB.y()) && (maxA.y() >= minB.y());
//     bool zOverlap = (minA.z() <= maxB.z()) && (maxA.z() >= minB.z());

//     if (!(xOverlap && yOverlap && zOverlap)) {
//         return false;
//     }

//     // Вычисляем глубину проникновения по каждой оси
//     QVector3D penetration;
//     penetration.setX(qMin(maxA.x() - minB.x(), maxB.x() - minA.x()));
//     penetration.setY(qMin(maxA.y() - minB.y(), maxB.y() - minA.y()));
//     penetration.setZ(qMin(maxA.z() - minB.z(), maxB.z() - minA.z()));

//     // Находим ось с минимальным проникновением (наибольшее перекрытие)
//     if (penetration.x() < penetration.y() && penetration.x() < penetration.z()) {
//         collisionNormal = QVector3D(maxA.x() < maxB.x() ? 1.0f : -1.0f, 0.0f, 0.0f);
//         penetrationDepth = penetration.x();
//     }
//     else if (penetration.y() < penetration.z()) {
//         collisionNormal = QVector3D(0.0f, maxA.y() < maxB.y() ? 1.0f : -1.0f, 0.0f);
//         penetrationDepth = penetration.y();
//     }
//     else {
//         collisionNormal = QVector3D(0.0f, 0.0f, maxA.z() < maxB.z() ? 1.0f : -1.0f);
//         penetrationDepth = penetration.z();
//     }

//     return true;
// }

#include "collisiondetector.h"
#include <QVector3D>
#include <cmath>
#include <QDebug>

QVector<CollisionInfo> CollisionDetector::detectCollisions(const QVector<PhysicalObject*>& bodies) {
    QVector<CollisionInfo> collisions;

    for (int i = 0; i < bodies.size(); ++i) {
        PhysicalObject* a = bodies[i];
        if (!a) continue;  // Пропускаем нулевые указатели

        for (int j = i + 1; j < bodies.size(); ++j) {
            PhysicalObject* b = bodies[j];
            if (!b) continue;  // Пропускаем нулевые указатели

            QVector3D minA, maxA, minB, maxB;
            a->getAABB(minA, maxA);
            b->getAABB(minB, maxB);

            QVector3D collisionNormal;
            float penetrationDepth;

            if (aabbCollision(minA, maxA, minB, maxB, collisionNormal, penetrationDepth)) {
                CollisionInfo info;
                info.bodyA = a;
                info.bodyB = b;
                info.collisionNormal = collisionNormal;
                info.penetrationDepth = penetrationDepth;
                collisions.append(info);
                a->setPosition(QVector3D(a->getPosition().x() + 1.0f, a->getPosition().y() + 0.0f, a->getPosition().z() + 0.0f));
                a->update(100);

                qDebug() << "Collision detected between shapes at positions:"
                         << a->getPosition() << "and" << b->getPosition()
                         << "Normal:" << collisionNormal
                         << "Penetration:" << penetrationDepth;
            }
        }
    }

    return collisions;
}

bool CollisionDetector::aabbCollision(const QVector3D &minA, const QVector3D &maxA,
                                      const QVector3D &minB, const QVector3D &maxB,
                                      QVector3D& collisionNormal, float& penetrationDepth) {
    // Проверка пересечения по осям
    bool xOverlap = (minA.x() <= maxB.x()) && (maxA.x() >= minB.x());
    bool yOverlap = (minA.y() <= maxB.y()) && (maxA.y() >= minB.y());
    bool zOverlap = (minA.z() <= maxB.z()) && (maxA.z() >= minB.z());

    if (!(xOverlap && yOverlap && zOverlap)) {
        return false;
    }

    // Вычисляем глубину проникновения по каждой оси
    QVector3D penetration;
    penetration.setX(qMin(maxA.x() - minB.x(), maxB.x() - minA.x()));
    penetration.setY(qMin(maxA.y() - minB.y(), maxB.y() - minA.y()));
    penetration.setZ(qMin(maxA.z() - minB.z(), maxB.z() - minA.z()));

    // Находим ось с минимальным проникновением (наибольшее перекрытие)
    if (penetration.x() < penetration.y() && penetration.x() < penetration.z()) {
        collisionNormal = QVector3D(maxA.x() < maxB.x() ? 1.0f : -1.0f, 0.0f, 0.0f);
        penetrationDepth = penetration.x();
    }
    else if (penetration.y() < penetration.z()) {
        collisionNormal = QVector3D(0.0f, maxA.y() < maxB.y() ? 1.0f : -1.0f, 0.0f);
        penetrationDepth = penetration.y();
    }
    else {
        collisionNormal = QVector3D(0.0f, 0.0f, maxA.z() < maxB.z() ? 1.0f : -1.0f);
        penetrationDepth = penetration.z();
    }

    return true;
}
