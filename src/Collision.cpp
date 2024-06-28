#include "Collision.hpp"
#include "ECS/Components/Collider.hpp"

bool Collision::collider_AABB(const Collider *CA, const Collider *CB){
    Transform& TA = CA->entity->getComponent<Transform>();  // Just to simplify.
    Transform& TB = CB->entity->getComponent<Transform>();
    if (TA.get_x() + CA->width  > TB.get_x() &&
        TB.get_x() + CB->width  > TA.get_x() &&
        TA.get_y() + CA->height > TB.get_y() &&
        TB.get_y() + CB->height > TA.get_y()){

        std::cout << CA->get_tag() << " hit: " << CB->get_tag() << "\n";
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
        CA->transform->get_x() + CA->width  > CB->transform->get_x()
        We want to get the limiting value of t in which we have an equality, instead of an inequality:
        CA->transform->get_x() + CA->width  = CB->transform->get_x()
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

            switch(col_handle_A){
            case (IMMOVABLE_ON_COLLISION):    // Stop B
                // // FIRST: CHECK PROJECTILE HANDLING BY A AND B.
                // if (CB->entity->has_group(ProjectileGroup)){
                // switch(proj_handle_A){
                // case(INDESTRUCTABLE_ON_PROJECTILE): break;  // Do nothing.
                // case(DESTROY_ON_PROJECTILE): CA->entity->destroy(); break;  // Destroy A
                // case(DAMAGE_ON_PROJECTILE):
                //     // TODO.
                //     break;
                // default:
                //     goto proj_error_case;
                // }}
                // // Now for B:
                // if (CA->entity->has_group(ProjectileGroup)){
                // switch(proj_handle_B){
                // case(INDESTRUCTABLE_ON_PROJECTILE): break;  // Do nothing.
                // case(DESTROY_ON_PROJECTILE): CB->entity->destroy(); break;  // Destroy A
                // case(DAMAGE_ON_PROJECTILE):
                //     // TODO.
                //     break;
                // default:
                //     goto proj_error_case;
                // }}

                speed_a = 0;    // Artificially set A's speed to 0 for calculations.
                switch (col_handle_B) {         
                // We exclude case(IMMOVABLE_ON_COLLISION) because it was already checked in line
                // (CA->handling_option + CB->handling_option == 0)
                case (MOVABLE_OBJECT):      // In both cases we'll stop B:
                case (PUSH_ON_COLLISION):
                    std::tie(diff, t_idx) = Collision::get_collision_time(
                        xa, xb, ya, yb, speed_a, speed_b, vxa, vxb, vya, vyb, wa, wb, ha, hb);

                    // Update ONLY B's transform :
                    if (t_idx <= 1){ 
                        TB.set_x( xb - speed_b*vxb* diff );
                        // CB->transform->set_vx(0.0); 
                    }else{ 
                        TB.set_y( yb - speed_b*vyb* diff );
                        // CB->transform->set_vy(0.0);
                    }
                    if (CB->entity->has_component<Sprite>()){ CB->entity->getComponent<Sprite>().update(); }
                    if (CB->entity->has_component<UILabel>()){ CB->entity->getComponent<UILabel>().update(); }
                    
                    break;
                case(DESTROY_ON_COLLISION):     // Destroy B
                    goto just_destroy_b;
                default:
                    goto error_case;
                }
                break;
            
            case(MOVABLE_OBJECT):
                switch (col_handle_B) {
                case (IMMOVABLE_ON_COLLISION):  // Stop A
                    goto stop_A;
                case (MOVABLE_OBJECT):          // Stop at the "middle".
                    goto stop_at_middle;
                case (PUSH_ON_COLLISION):       // Move B
                    /// TODO
                    break;
                case(DESTROY_ON_COLLISION):     // Destroy B
                    goto just_destroy_b;
                default:
                    goto error_case;
                }
                break;

            case(PUSH_ON_COLLISION):
                switch (col_handle_B) {
                case (IMMOVABLE_ON_COLLISION):    // Stop A
                stop_A:
                    speed_b = 0;
                    std::tie(diff, t_idx) = Collision::get_collision_time(
                        xa, xb, ya, yb, speed_a, speed_b, vxa, vxb, vya, vyb, wa, wb, ha, hb);

                    // Update ONLY A's transform :
                    if (t_idx <= 1){ 
                        TA.set_x( xa - speed_a*vxa* diff );
                        // CA->transform->set_vx(0.0); 
                    }else{ 
                        TA.set_y( ya - speed_a*vya* diff );
                        // CA->transform->set_vy(0.0); 
                    }
                    if (CA->entity->has_component<Sprite>()){ CA->entity->getComponent<Sprite>().update(); }
                    if (CA->entity->has_component<UILabel>()){ CA->entity->getComponent<UILabel>().update(); }
                
                    break;
                case (MOVABLE_OBJECT):      // Move A
                    /// TODO
                    break;
                case (PUSH_ON_COLLISION):   // Stop at the "middle".
                stop_at_middle: // Regular time calculation, updating both entities:
                    std::tie(diff, t_idx) = Collision::get_collision_time(
                        xa, xb, ya, yb, speed_a, speed_b, vxa, vxb, vya, vyb, wa, wb, ha, hb);

                    // Update A:
                    if (t_idx <= 1){
                        TA.set_x( xa - speed_a*vxa* diff );
                        // CA->transform->set_vx(0.0);
                    }else{
                        TA.set_y( ya - speed_a*vya* diff );
                        // CA->transform->set_vy(0.0);
                    }
                    if (CA->entity->has_component<Sprite>()){ CA->entity->getComponent<Sprite>().update(); }
                    if (CA->entity->has_component<UILabel>()){ CA->entity->getComponent<UILabel>().update(); }
                
                    // Update B:
                    TB.set_x( xb - speed_b*vxb* diff );
                    TB.set_y( yb - speed_b*vyb* diff );
                    if (t_idx <= 1){ TB.set_vx(0.0); }else{ TB.set_vy(0.0); }
                    if (CB->entity->has_component<Sprite>()){ CB->entity->getComponent<Sprite>().update(); }
                    if (CB->entity->has_component<UILabel>()){ CB->entity->getComponent<UILabel>().update(); }
                
                    break;
                case(DESTROY_ON_COLLISION): // Destroy B
                    goto just_destroy_b;
                    break;
                default:
                    goto error_case;
                }
                break;

            case(DESTROY_ON_COLLISION): // Destroy A but:
                CA->entity->destroy();
                switch (col_handle_B) {
                case (DESTROY_ON_COLLISION):    // In this case, also destroy B.
                just_destroy_b:
                    CB->entity->destroy();
                    break;
                case (IMMOVABLE_ON_COLLISION): 
                    break;   
                case (MOVABLE_OBJECT):
                    break;
                case (PUSH_ON_COLLISION):
                    break;
                default:
                    goto error_case;
                }
                break;

            default:  // Unnexpected case: throw an error:
            error_case:
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
