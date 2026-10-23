#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import PoseStamped

# L'API Python officielle de MoveIt 2
from moveit.planning import MoveItPy

def main(args=None):
    rclpy.init(args=args)
    
    # Initialisation du nœud MoveItPy
    # Il va automatiquement lire la configuration de votre robot (URDF/SRDF)
    hercule_arm = MoveItPy(node_name="moveit_py_node")
    
    # On récupère le groupe de planification (celui que vous avez nommé "arm")
    planning_component = hercule_arm.get_planning_component("arm")
    
    # 1. Définir la cible en coordonnées cartésiennes (X, Y, Z)
    target_pose = PoseStamped()
    target_pose.header.frame_id = "base_link"  # Remplacez par le nom exact du link de votre base
    
    # Position cible en mètres
    target_pose.pose.position.x = 0.3
    target_pose.pose.position.y = 0.1
    target_pose.pose.position.z = 0.4
    
    # Orientation cible (quaternion neutre pour cet exemple)
    target_pose.pose.orientation.w = 1.0 
    target_pose.pose.orientation.x = 0.0
    target_pose.pose.orientation.y = 0.0
    target_pose.pose.orientation.z = 0.0

    # Assigner la cible à l'effecteur (remplacez "tip_link" par le dernier link de votre bras)
    planning_component.set_goal_state(pose_stamped_msg=target_pose, pose_link="tip_link")
    
    # 2. Planifier la trajectoire (Calcul de la cinématique inverse)
    print("Calcul des angles nécessaires en cours...")
    plan_result = planning_component.plan()
    
    # 3. Exécuter le mouvement si la planification a réussi
    if plan_result:
        print("Chemin trouvé ! Exécution du mouvement...")
        hercule_arm.execute(plan_result.trajectory, controllers=[])
    else:
        print("Échec : Impossible de trouver une solution pour atteindre ces coordonnées.")
        
    rclpy.shutdown()

if __name__ == '__main__':
    main()
