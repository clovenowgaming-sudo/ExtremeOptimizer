#include <Geode/Geode.hpp>
#include <Geode/modify/CCParticleSystemQuad.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/HardStreak.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/RingObject.hpp>
#include <Geode/modify/GhostTrailEffect.hpp>
#include <Geode/modify/CCCircleWave.hpp>

using namespace geode::prelude;

// Visual only. Physics, triggers, hitboxes and TPS are never touched.

static bool opt(char const* key) {
    return Mod::get()->getSettingValue<bool>(key);
}

static bool active() {
    if (!opt("only-in-level")) return true;
    return PlayLayer::get() != nullptr;
}

class $modify(EOParticle, CCParticleSystemQuad) {
    void draw() {
        if (opt("disable-particles") && active()) return;
        CCParticleSystemQuad::draw();
    }
};

class $modify(EOStreak, CCMotionStreak) {
    void draw() {
        if (opt("disable-streak") && active()) return;
        CCMotionStreak::draw();
    }
};

class $modify(EOBase, GJBaseGameLayer) {
    CCParticleSystemQuad* createParticle(int id, char const* plist, int zOrder, tCCPositionType type) {
        auto p = GJBaseGameLayer::createParticle(id, plist, zOrder, type);
        if (p && active()) {
            int pct = static_cast<int>(Mod::get()->getSettingValue<int64_t>("particle-amount"));
            if (pct < 100) {
                p->setEmissionRate(p->getEmissionRate() * static_cast<float>(pct) / 100.f);
            }
        }
        return p;
    }

    void shakeCamera(float duration, float strength, float interval) {
        if (opt("disable-shake") && active()) return;
        GJBaseGameLayer::shakeCamera(duration, strength, interval);
    }
};

class $modify(EOPlay, PlayLayer) {
    void setupHasCompleted() {
        PlayLayer::setupHasCompleted();
        if (opt("hide-background") && m_background) m_background->setVisible(false);
        if (opt("hide-ground") && m_groundLayer) m_groundLayer->setVisible(false);
    }
};

class $modify(EOPlayer, PlayerObject) {
    void playDeathEffect() {
        if (opt("disable-death-effect") && active()) return;
        PlayerObject::playDeathEffect();
    }
};

class $modify(EOWave, HardStreak) {
    void updateStroke(float dt) {
        if (opt("disable-wave-trail") && active()) return;
        HardStreak::updateStroke(dt);
    }
};

class $modify(EORing, RingObject) {
    void spawnCircle() {
        if (opt("disable-orb-effect") && active()) return;
        RingObject::spawnCircle();
    }
};

class $modify(EOObject, GameObject) {
    void addGlow(gd::string frame) {
        if (active()) {
            if (opt("disable-all-glow")) return;
            if (opt("disable-spike-glow") && m_objectType == GameObjectType::Hazard) return;
            if (opt("disable-block-glow") && m_objectType == GameObjectType::Solid) return;
        }
        GameObject::addGlow(frame);
    }

    void playShineEffect() {
        if (opt("disable-pad-glow") && active()) return;
        GameObject::playShineEffect();
    }

    void activateObject() {
        if (opt("static-orbs") && active()) {
            m_usesAudioScale = false;
        }
        GameObject::activateObject();
        if (opt("hide-decoration") && active() && m_objectType == GameObjectType::Decoration) {
            this->setVisible(false);
        }
    }
};

class $modify(EOGhost, GhostTrailEffect) {
    void trailSnapshot(float dt) {
        if (opt("disable-ghost-trail") && active()) return;
        GhostTrailEffect::trailSnapshot(dt);
    }
};

class $modify(EOWaveRing, CCCircleWave) {
    void draw() {
        if (opt("disable-circle-wave") && active()) return;
        CCCircleWave::draw();
    }
};

class $modify(EOPower, GameObject) {
    void powerOnObject(int id) {
        if (active()) {
            bool isRing = typeinfo_cast<RingObject*>(this) != nullptr;
            bool isPad = m_objectType == GameObjectType::YellowJumpPad
                || m_objectType == GameObjectType::PinkJumpPad
                || m_objectType == GameObjectType::GravityPad
                || m_objectType == GameObjectType::RedJumpPad;
            if (isRing && opt("disable-orb-effect")) return;
            if (isPad && opt("disable-pad-glow")) return;
        }
        GameObject::powerOnObject(id);
    }
};
