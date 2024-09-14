#include "Collision.hpp"
#include "ECS/Components/Collider.hpp"
#include "CustomControllers.hpp"

bool Collision::collider_AABB(const Collider *CA, const Collider *CB){
    Transform& TA = CA->entity->getComponent<Transform>();  // Just to simplify.
    Transform& TB = CB->entity->getComponent<Transform>();
    if (TA.x + CA->width  > TB.x &&
        TB.x + CB->width  > TA.x &&
        TA.y + CA->height > TB.y &&
        TB.y + CB->height > TA.y){
        
        #ifdef DEBUG_MODE
            std::cout << CA->tag << " hit: " << CB->tag << std::endl;
        #endif 
        return true;
    }else{ return false; }
}


std::tuple<float, int> Collision::get_collision_time( 
        float xa,  float xb,  float ya,  float yb, float speed_a, float speed_b,
        float vxa, float vxb, float vya, float vyb, 
        float wa,  float wb,  float ha,  float hb)
{
        std::array<float, 4> t;     // Array for storing time values for each condition
        float min_val = 100000;     // Smallest time value between the calculated ones
        int best_idx = -1;          // Index of the condition (intialized at illogical value)
        float dvx = speed_b*vxb - speed_a*vxa;  // Denominators (for checking division by zero)
        float dvy = speed_b*vyb - speed_a*vya; 

        // Checking denominators for avoiding division by zero:
        if (dvx == 0 && dvy == 0){  // Both denominators equal zero
            /** This would mean that both objects are moving at the same speed in the same direction
            (including the trivial case of speeds = 0). This shouldn't be possible unless an object
            has spawned inside of another one. Let's throw an error for debugging this: */
            std::cout << "Error calculating collision: speed difference = 0" << std::endl;
            throw std::runtime_error("Game::update");
        }
        // Otherwise, if one of the denominators is zero, increate the t value artificially,
        // avoiding division by zero.
        if (dvx == 0){ dvx = 0.000001; } if (dvy == 0) { dvy = 0.000001; }

        /** Calculating times (based in the conditions in Collision::collider_AABB, where
        the collider's x and y have been switched by:
        x = x0 - speed * vx * t
        y = y0 - speed * vy * t
        Here, (x0, y0) is the current position of the object (when the collision is happening),
        which is either (xa, ya) or (xb, yb).
        We have a MINUS sign because we want to calculate the time we have to move backwards 
        (which equates to switching the sign of vx or vy).

        For the first condition:
        CA's transform->get_x() + CA->width > CB's transform->get_x()
        We want to get the limiting value of t in which we have an equality, instead of an inequality:
        CA's transform->get_x() + CA->width = CB's transform->get_x()
        This gives us:
        xa - speed_a * vxa * t + wa = xb - speed_b * vxb * t
        t = (xa - xb + wa)/(- speed_b*vxb + speed_a*vxa)        // Definition of t[0], below

        This can be repeated for all 4 conditions defined in Collision::collider_AABB. Since
        ALL conditions must be met for the collision to happen, we only need that ONE of the
        conditions IS NOT met for the collision to be over. Hence, we'll later get the SMALLEST
        value of t amongst the 4, excluding negative values (which would mean going forwards in
        time).
        */
        t[0] = (xa - xb + wa)/(-dvx); 
        t[1] = (xb - xa + wb)/(dvx); 
        t[2] = (ya - yb + ha)/(-dvy); 
        t[3] = (yb - ya + hb)/(dvy); 

        for (int idx = 0; idx < 4; idx++){          // Getting the minimum, positive value
            if (t[idx] < min_val && t[idx] > 0){ min_val = t[idx]; best_idx = idx; } }

        #ifdef DEBUG_MODE
            if (min_val == 100000 || best_idx == -1){   // Couldn't find a valid t.
                std::cout << "Error calculating collision: min_val and best_idx" << std::endl;
                throw std::runtime_error("Game::update");
            }
        #endif

        return {min_val, best_idx};
    }

void Collision::handle_collisions(){
    /// TODO: This loop can definitely be optimized somehow.
    int nb_colliders = Game::collider_vector.size();
    for (int i = 0; i < nb_colliders - 1; i++){          // From 0 to N-1
    for (int j = i+1; j < nb_colliders; j++){        // From I+1 to N
        Collider* CA = Game::collider_vector[i]; Collider* CB = Game::collider_vector[j];

        // If both collision handles are immovable:
        if (CA->handling_option == CollisionHandle::IMMOVABLE 
        &&  CB->handling_option == CollisionHandle::IMMOVABLE){
            continue; //----CONTINUE----CONTINUE----CONTINUE----CONTINUE----CONTINUE----CONTINUE----CONTINUE----CONTINUE
        }
        // If we got here, then at least one of the objects is not immovable, 
        // let's check for collision:
        bool collision_result = Collision::collider_AABB(CA, CB);
        if(!collision_result){ continue; } // If there's no collision, we continue ----CONTINUE----CONTINUE----CONTINUE----CONTINUE
        
        // We got a collision!
        // Let's check the special cases:
        Entity*& EA = CA->entity; Entity*& EB = CB->entity;
        // 1. where we have a cursor:
        // must check
        ComponentID id_A = has_mouse_controller(*EA);
        ComponentID id_B = has_mouse_controller(*EB);
        bool condition1 = EA->name=="CURSOR" && id_B != max_components + 1;
        bool condition2 = EB->name=="CURSOR" && id_A != max_components + 1;
        if (condition1 || condition2){
            if (condition1){
                get_custom_mouse_controller(*EB, id_B)->handle_events();
            }else{
                get_custom_mouse_controller(*EA, id_A)->handle_events();
            }
        }

        // 2. Let's check if entities are active and if one is damaging the other:
        condition1 = EA->is_active() && EA->has_component<Health>() && EB->has_component<Damage>();
        condition2 = EB->is_active() && EB->has_component<Health>() && EA->has_component<Damage>();
        if (condition1 || condition2){  // Note that BOTH can be true.
            std::vector<Entity*> immune_vec;
            if (condition1){    // EA will take damage if not immune.
                immune_vec = EB->getComponent<Damage>().immune;
                int count = std::count(immune_vec.begin(), immune_vec.end(), EA);
                if (count == 0){// EA isn't immune: damage it
                    EA->getComponent<Health>().update_hp(EB->getComponent<Damage>().damage); }
            }

            if (condition2){    // EB will take damage if not immune.
                immune_vec = EA->getComponent<Damage>().immune;
                int count = std::count(immune_vec.begin(), immune_vec.end(), EB);
                if (count == 0){// EB isn't immune: damage it
                    EB->getComponent<Health>().update_hp(EA->getComponent<Damage>().damage); }
            }
        }

        // We still want to handle collision in case we need to destroy one of the colliders on impact
        if (!EA->is_active() && !EB->is_active()){ continue; } //< No need to check collision if one of them is inactive, we continue----CONTINUE----CONTINUE----CONTINUE----CONTINUE
        
        //Let's handle it according to the collider options:
        CollisionHandle col_handle_A = CA->handling_option, col_handle_B = CB->handling_option;
        std::size_t proj_handle_A = CA->projectile_handling_option, proj_handle_B = CB->projectile_handling_option;

        /* To handle collision, we'll move the objects "back in time", until there was no
        collision. Looking into "Collision::collider_AABB", we'll see a series of logical
        cases that need to be met for a collision to take place. We want to move back in
        time to the limit case in which (at least) one of those conditions is not met. We
        can calculate that time in a straightforward way for each of the logical conditions.
        For the first one: 
        CA->transform->get_x() + CA->width  >= CB->transform->get_x() // we'll write as :
        XA + WA >= XB
        In the edge case where there's no collision:
        XA + WA = XB.
        Considering constant velocity (which is the current case in our implementation):
        X = VX * t + T0
        VXA * t + XA0 + WA = VXB * t + XB0
        t = (XA0 - XB0 + WA)/(VBX - VAX)
        t is the time in which this condition is not met. Since all four conditions have to
        be met for there to be a collision, we have to calculate this t for all four, and pick
        the earliest one.
        Note that we need to consider the opposite of the entity's velocities (because we are
        "going back in time"). XA0 and XB0 would be the current positions.
        */
        
        // For clarity:
        Transform& TA = CA->entity->getComponent<Transform>();
        Transform& TB = CB->entity->getComponent<Transform>();
        float xa = TA.x; float ya = TA.y;
        float xb = TB.x; float yb = TB.y;
        float speed_a = TA.speed;
        float speed_b = TB.speed;
        float vxa = TA.vx; float vya = TA.vy;
        float vxb = TB.vx; float vyb = TB.vy;
        float wa = CA->width;
        float ha = CA->height;
        float wb = CB->width;
        float hb = CB->height;

        float diff; // Amount of time to move.
        int t_idx;  // Information about collision axis.


        /* Here, I'll use bit manipulation to avoid code duplication: it
        will allow us to have only one switch statement for the combination
        of cases. Adapted from: 
        https://stackoverflow.com/questions/8644096/multiple-conditions-in-switch-case
        */

        #define IMMOVABLE_BIT (1 << 0)  // 00001
        #define MOVABLE_BIT (1 << 1)    // 00010
        #define PUSH_BIT (1 << 2)       // 00100
        #define PHASE_BIT (1 << 3)      // 01000
        #define DESTROY_BIT (1 << 4)    // 10000

        // Here, we'll integrate the conditions to an int, using bitwise OR (|):
        int bit_result = \
                ((col_handle_A == CollisionHandle::IMMOVABLE) or
                (col_handle_B == CollisionHandle::IMMOVABLE) ? IMMOVABLE_BIT : 0) 
            | ((col_handle_A == CollisionHandle::MOVABLE) or
                (col_handle_B == CollisionHandle::MOVABLE) ? MOVABLE_BIT : 0) 
            | ((col_handle_A == CollisionHandle::PUSH) or
                (col_handle_B == CollisionHandle::PUSH) ? PUSH_BIT : 0) 
            | ((col_handle_A == CollisionHandle::PHASE) or
                (col_handle_B == CollisionHandle::PHASE) ? PHASE_BIT : 0) 
            | ((col_handle_A == CollisionHandle::DESTROY) or
                (col_handle_B == CollisionHandle::DESTROY) ? DESTROY_BIT : 0);
        #ifdef DEBUG_MODE
            std::cout << "Bit result: " << bit_result << std::endl;
        #endif
        switch(bit_result){
        // Here, the cases in which one of them is CollisionHandle::IMMOVABLE:
        case (IMMOVABLE_BIT):   // Both objects Immovable
            // Already checked before:
            // "We exclude case(CollisionHandle::IMMOVABLE) because it was already checked in line
            // (CA->handling_option + CB->handling_option == 0)"
            // Weird that we got here:
            throw std::runtime_error(
                "Arrived at IMMOVABLE_BIT case in switch statement even though it had been tested for.");
            break;
        case (IMMOVABLE_BIT + MOVABLE_BIT):     // fall through
        case (IMMOVABLE_BIT + PUSH_BIT):
            Collider* C_MOV; Transform* T_MOV;
            float x_mov, y_mov, speed_mov, vx_mov, vy_mov;
            if (col_handle_A == CollisionHandle::IMMOVABLE){    // A is the IMMOVABLE one:
                C_MOV = CB; T_MOV = &TB; speed_a = 0;       // Must set speed of immovable to 0 for time calculation
                x_mov = xb; y_mov = yb; speed_mov = speed_b;//  (equates to not moving the immovable back in time). 
                vx_mov= vxb; vy_mov = vyb;
            } else{                                         // B is the IMMOVABLE one
                C_MOV = CA; T_MOV = &TA; speed_b = 0;       // Must set speed of immovable to 0 for time calculation
                x_mov = xa; y_mov = ya; speed_mov = speed_a;// (equates to not moving the immovable back in time).
                vx_mov= vxa; vy_mov = vya;
            }
            // TODO: if we have objects that are IMMOVABLE ON COLLISION but are actually moving,
            // This will have to be improved (we can't just set the speed of the immovable object
            // to zero like here, we'll have not only to get collision time with accurate speeds,
            // we'll have to compensate movement in t \in [collision_time, current_time]).
            // This isn't too difficult to do: Calculate collision time and push the movable object's
            // position to that state (do not update immovable object at all since it is immovable, and
            // will end up in the place it is either way). Then, calculate the angle of approach between
            // objects, and either reflect the movable object's v according to the normal and calculate 
            // displacement, or set the movable object's v to the immovable object's v. These are still not
            // exactly accurate. We have to make remove from v_mov the component of v_immov, (and perhaps
            // adapt the speed accordingly (?)). Ok, that is a little more complicated.
            std::tie(diff, t_idx) = Collision::get_collision_time(
                    xa, xb, ya, yb, speed_a, speed_b, vxa, vxb, vya, vyb, wa, wb, ha, hb);

            // Update ONLY C_MOV's transform :
            if (t_idx <= 1){ T_MOV->x = x_mov - speed_mov*vx_mov* diff; }
            else{ T_MOV->y = y_mov - speed_mov*vy_mov* diff; }
            if (C_MOV->entity->has_component<Sprite>()){ C_MOV->entity->getComponent<Sprite>().update(); }
            if (C_MOV->entity->has_component<UILabel>()){ C_MOV->entity->getComponent<UILabel>().update(); }
            break;
        case (IMMOVABLE_BIT + PHASE_BIT):
            goto do_nothing;
            break; // just in case
        case (IMMOVABLE_BIT + DESTROY_BIT): 
            goto destroy_one;
            break; // just in case

        // Now cases in which one of them has the CollisionHandle::MOVABLE option:
        case (MOVABLE_BIT): // Both are MOVABLE.
            goto stop_at_middle;
            break; // just in case
        case (MOVABLE_BIT + PUSH_BIT):
            // TODO: push the pushable object.
            break;
        case (MOVABLE_BIT + PHASE_BIT):
            goto do_nothing;
        case (MOVABLE_BIT + DESTROY_BIT):
            goto destroy_one;
            break; // just in case

        // Now cases in which one of them has the CollisionHandle::PUSH option:
        case (PUSH_BIT):    // Both are CollisionHandle::PUSH, BUT also fall through of both MOVABLE (done through a goto).
        stop_at_middle: // Regular time calculation, updating both entities:
            std::tie(diff, t_idx) = Collision::get_collision_time(
                xa, xb, ya, yb, speed_a, speed_b, vxa, vxb, vya, vyb, wa, wb, ha, hb);

            // Update A and B:
            if (t_idx <= 1){ 
                TA.x = xa - speed_a*vxa* diff; TB.x = xb - speed_b*vxb* diff; 
            }else{
                TA.y = ya - speed_a*vya* diff; TB.y = yb - speed_b*vyb* diff;
            }
            if (CA->entity->has_component<Sprite>()){ CA->entity->getComponent<Sprite>().update(); }
            if (CA->entity->has_component<UILabel>()){ CA->entity->getComponent<UILabel>().update(); }
            if (CB->entity->has_component<Sprite>()){ CB->entity->getComponent<Sprite>().update(); }
            if (CB->entity->has_component<UILabel>()){ CB->entity->getComponent<UILabel>().update(); }
            break;
        case (PUSH_BIT + PHASE_BIT):
            goto do_nothing;
            break;  // just in case
        case (PUSH_BIT + DESTROY_BIT):
            goto destroy_one;
            break;

        // Cases with PHASE_BIT:
        case (PHASE_BIT):
        do_nothing:
            break;
        
        case (PHASE_BIT + DESTROY_BIT):
        destroy_one:    // We'll destroy the one with the CollisionHandle::DESTROY option.
            if (col_handle_A == CollisionHandle::DESTROY){ CA->entity->destroy(); } 
            else { CB->entity->destroy(); }
            break;

        // Cases with DESTROY_BIT:
        case (DESTROY_BIT): // Both have the CollisionHandle::DESTROY: Destroy both entities.    
            CA->entity->destroy(); CB->entity->destroy();
            break;
        
        default:  // Unnexpected case: throw an error:
            #ifdef DEBUG_MODE
                std::cout << "Unknown case in collision detection. Options A and B: " \
                            << static_cast<std::size_t>(col_handle_A) << "; " << static_cast<std::size_t>(col_handle_B )<< "." << std::endl;
                std::cout << "Entities: " << CA->entity->get_name() << "; " \
                            << CB->entity->get_name() << std::endl;
                throw std::runtime_error("Game.cpp: Game::update");
            #endif
            break;
        }
    }}
}
