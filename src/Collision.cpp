#include "Collision.hpp"
#include "ECS/Components/Collider.hpp"

bool Collision::collider_AABB(const Collider *CA, const Collider *CB){
    Transform& TA = CA->entity->getComponent<Transform>();  // Just to simplify.
    Transform& TB = CB->entity->getComponent<Transform>();
    if (TA.get_x() + CA->width  > TB.get_x() &&
        TB.get_x() + CB->width  > TA.get_x() &&
        TA.get_y() + CA->height > TB.get_y() &&
        TB.get_y() + CB->height > TA.get_y()){

        std::cout << CA->get_tag() << " hit: " << CB->get_tag() << std::endl;
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
        else if (dvx == 0){ dvx = 0.000001; } else if (dvy == 0) { dvy = 0.000001; }

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

        if (min_val == 100000 || best_idx == -1){   // Couldn't find a valid t.
            std::cout << "Error calculating collision: min_val and best_idx" << std::endl;
            throw std::runtime_error("Game::update");
        }

        return {min_val, best_idx};
    }

void Collision::handle_collisions(){
    /// TODO: This loop can definitely be optimized somehow.
    int nb_colliders = Game::collider_vector.size();
    for (int i = 0; i < nb_colliders - 1; i++){          // From 0 to N-1
    for (int j = i+1; j < nb_colliders; j++){        // From I+1 to N
        Collider* CA = Game::collider_vector[i]; Collider* CB = Game::collider_vector[j];

        if (CA->handling_option + CB->handling_option == 0){
            // Small trick for checking if both colliders are immovable more quickly:
            // Instead of checking if each is IMMOVABLE_ON_COLLISION, we check if their sum is one,
            // since IMMOVABLE_ON_COLLISION is the enum's entry for 0.
            continue;
        }else if (CA->entity->has_group(PlayerGroup) && CB->entity->has_group(PlayerGroup)){
            // This is checked separately because it is less frequent than the previous one.
            continue;
        }
        // If we got here, then at least one of the objects is movable, let's check for
        // collision:
        bool collision_result = Collision::collider_AABB(CA, CB);
        if(collision_result){
            // We got a collision, let's handle it according to the collider options:
            std::size_t col_handle_A = CA->handling_option, col_handle_B = CB->handling_option;
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
            float xa = TA.get_x();
            float ya = TA.get_y();
            float xb = TB.get_x();
            float yb = TB.get_y();
            float speed_a = TA.get_speed();
            float speed_b = TB.get_speed();
            float vxa = TA.get_vx();
            float vya = TA.get_vy();
            float vxb = TB.get_vx();
            float vyb = TB.get_vy();
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

            #define IMMOVABLE_BIT (1 << 0)  // 0001
            #define MOVABLE_BIT (1 << 1)    // 0010
            #define PUSH_BIT (1 << 2)       // 0100
            #define DESTROY_BIT (1 << 3)    // 1000

            // Here, we'll integrate the conditions to an int, using bitwise OR:
            int bit_result = \
                  ((col_handle_A == IMMOVABLE_ON_COLLISION) or
                   (col_handle_B == IMMOVABLE_ON_COLLISION) ? IMMOVABLE_BIT : 0) 
                | ((col_handle_A == MOVABLE_OBJECT) or
                   (col_handle_B == MOVABLE_OBJECT) ? MOVABLE_BIT : 0) 
                | ((col_handle_A == PUSH_ON_COLLISION) or
                   (col_handle_B == PUSH_ON_COLLISION) ? PUSH_BIT : 0) 
                | ((col_handle_A == DESTROY_ON_COLLISION) or
                   (col_handle_B == DESTROY_ON_COLLISION) ? DESTROY_BIT : 0);

            std::cout << "Bit result: " << bit_result << std::endl;
            switch(bit_result){
            // Here, the cases in which one of them is IMMOVABLE_ON_COLLISION:
            case (IMMOVABLE_BIT):   // Both objects Immovable
                // Already checked before:
                // "We exclude case(IMMOVABLE_ON_COLLISION) because it was already checked in line
                // (CA->handling_option + CB->handling_option == 0)"
                // Weird that we got here:
                throw std::runtime_error(
                    "Arrived at IMMOVABLE_BIT case in switch statement even though it had been tested for.");
                break;
            case (IMMOVABLE_BIT + MOVABLE_BIT):     // fall through
            case (IMMOVABLE_BIT + PUSH_BIT):
                Collider* C_MOV; Transform* T_MOV;
                float x_mov, y_mov, speed_mov, vx_mov, vy_mov;
                if (col_handle_A == IMMOVABLE_ON_COLLISION){    // A is the IMMOVABLE one:
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
                if (t_idx <= 1){ T_MOV->set_x( x_mov - speed_mov*vx_mov* diff ); }
                else{ T_MOV->set_y( y_mov - speed_mov*vy_mov* diff ); }
                if (C_MOV->entity->has_component<Sprite>()){ C_MOV->entity->getComponent<Sprite>().update(); }
                if (C_MOV->entity->has_component<UILabel>()){ C_MOV->entity->getComponent<UILabel>().update(); }
                break;
            case (IMMOVABLE_BIT + DESTROY_BIT): 
                goto destroy_one;
                break; // just in case

            // Now cases in which one of them has the MOVABLE_OBJECT option:
            case (MOVABLE_BIT): // Both are MOVABLE.
                goto stop_at_middle;
                break; // just in case
            case (MOVABLE_BIT + PUSH_BIT):
                // TODO: push the pushable object.
                break;
            case (MOVABLE_BIT + DESTROY_BIT):
                goto destroy_one;
                break; // just in case

            // Now cases in which one of them has the PUSH_ON_COLLISION option:
            case (PUSH_BIT):    // Both are PUSH_ON_COLLISION, BUT also fall through of both MOVABLE (done through a goto).
            stop_at_middle: // Regular time calculation, updating both entities:
                std::tie(diff, t_idx) = Collision::get_collision_time(
                    xa, xb, ya, yb, speed_a, speed_b, vxa, vxb, vya, vyb, wa, wb, ha, hb);

                // Update A:
                if (t_idx <= 1){ TA.set_x( xa - speed_a*vxa* diff ); }
                else{ TA.set_y( ya - speed_a*vya* diff ); }
                if (CA->entity->has_component<Sprite>()){ CA->entity->getComponent<Sprite>().update(); }
                if (CA->entity->has_component<UILabel>()){ CA->entity->getComponent<UILabel>().update(); }
            
                // Update B:
                TB.set_x( xb - speed_b*vxb* diff );
                TB.set_y( yb - speed_b*vyb* diff );
                if (t_idx <= 1){ TB.set_vx(0.0); }
                else{ TB.set_vy(0.0); }
                if (CB->entity->has_component<Sprite>()){ CB->entity->getComponent<Sprite>().update(); }
                if (CB->entity->has_component<UILabel>()){ CB->entity->getComponent<UILabel>().update(); }
                break;
            case (PUSH_BIT + DESTROY_BIT):
            destroy_one:    // We'll destroy the one with the DESTROY_ON_COLLISION option.
                if (col_handle_A == DESTROY_ON_COLLISION){ CA->entity->destroy(); } 
                else { CB->entity->destroy(); }
                break;

            // Cases with DESTROY_BIT:
            case (DESTROY_BIT): // Both have the DESTROY_ON_COLLISION: Destroy both entities.    
                CA->entity->destroy(); CB->entity->destroy();
                break;
            
            default:  // Unnexpected case: throw an error:
                std::cout << "Unknown case in collision detection. Options A and B: " \
                            << col_handle_A << "; " << col_handle_B << "." << std::endl;
                std::cout << "Entities: " << CA->entity->get_name() << "; " \
                            << CB->entity->get_name() << std::endl;
                throw std::runtime_error("Game.cpp: Game::update");
                break;
            }
        }
    }}
}
