"use client";

import { motion } from "framer-motion";

const Photo = () => {
  return (
    <div className="w-full h-full flex justify-center items-center relative">
      <motion.div
        initial={{ opacity: 0 }}
        animate={{
          opacity: 1,
          transition: { delay: 2, duration: 0.4, ease: "easeIn" },
        }}
        className="relative flex justify-center items-center"
      >
        {/* Animated Circle */}
        <motion.svg
          className="w-[250px] xl:w-[350px] h-[250px] xl:h-[350px]"
          fill="transparent"
          viewBox="0 0 506 506"
          xmlns="http://www.w3.org/2000/svg"
        >
          <motion.circle
            cx="253"
            cy="253"
            r="190"
            stroke="#292522"
            strokeWidth="4"
            strokeLinecap="round"
            strokeLinejoin="round"
            initial={{ strokeDasharray: "24 10 0 0" }}
            animate={{
              strokeDasharray: ["15 120 25 25", "16 25 92 72", "4 250 22 22"],
              rotate: [120, 360],
            }}
            transition={{
              duration: 20,
              repeat: Infinity,
              repeatType: "reverse",
            }}
          />
        </motion.svg>

        {/* Bigger Image (Centered in Circle) */}
        <motion.div
          initial={{ opacity: 0, scale: 0.8 }}
          animate={{
            opacity: 1,
            scale: 1,
            transition: { delay: 2.4, duration: 0.4, ease: "easeInOut" },
          }}
          className="absolute w-[180px] h-[180px] xl:w-[250px] xl:h-[250px] 
                     rounded-full bg-contain bg-no-repeat bg-center"
          style={{ backgroundImage: "url('/assets/photo.png')" }}
        ></motion.div>
      </motion.div>
    </div>
  );
};

export default Photo;
