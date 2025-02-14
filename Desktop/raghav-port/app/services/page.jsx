"use client";

import React, { useState, useEffect, useRef } from "react";
import Typewriter from "typewriter-effect";

const Services = () => {
  const [isVisible, setIsVisible] = useState(false);
  const typingSectionRef = useRef(null);

  useEffect(() => {
    const handleScroll = () => {
      if (typingSectionRef.current) {
        const rect = typingSectionRef.current.getBoundingClientRect();
        if (rect.top <= window.innerHeight && rect.bottom >= 0) {
          setIsVisible(true);
        }
      }
    };
    window.addEventListener("scroll", handleScroll);
    handleScroll();

    return () => {
      window.removeEventListener("scroll", handleScroll);
    };
  }, []);

  return (
    <section className="about py-16 text-center mb-64 flex justify-between items-start relative" id="about">
      {/* Left Section: About Me */}
      <div className="flex-1 max-w-[50%]">
        <h2 className="heading text-4xl font-bold mb-6 text-center mt-[20px]">
          About <span className="text-accent">Me</span>
        </h2>

        {/* Typewriter effect with pre-allocated height */}
        <div
          ref={typingSectionRef}
          className="max-w-[600px] mx-auto text-justify text-xl font-medium leading-relaxed text-gray-800 whitespace-pre-wrap overflow-hidden min-h-[360px]" // Ensures a static space
        >
          {isVisible && (
            <Typewriter
              options={{
                strings: [
                  'I am a Computer Science student at UC Santa Cruz with a strong foundation in software engineering, machine learning, and AI. As a Software Engineer Co-Op Intern at Nutanix, I optimize GenAI inferencing with SIMD instructions and develop performance-monitoring dashboards to enhance efficiency. My research at AIEA Lab focuses on improving Transformer models and integrating AI with logic programming for enhanced inference and decision-making. Previously, as a Software Engineer Intern at Tech4Good, I built responsive UI components and implemented state management for a goal-setting app. Passionate about AI, system design, and full-stack development, I enjoy solving complex problems, optimizing performance, and building scalable applications that drive innovation.'
                ],
                autoStart: true,
                loop: false,
                deleteSpeed: Infinity,
                delay: 10,
              }}
            />
          )}
        </div>
      </div>

      {/* Right Section: Cards */}
      {/* Right Section: Cards */}
      {/* Right Section: Cards */}
<div className="flex flex-col items-center space-y-8 absolute top-[64%] left-1/2 transform translate-x-[50%] -translate-y-1/2">
  {/* Card 1: UC Santa Cruz Logo (Increased Height) */}
  <div className="w-80 h-40 p-4 rounded-lg  flex items-center justify-center"> {/* Increased height here */}
    <img
      src="/assets/uc.png" // UCSC logo
      alt="UC Santa Cruz Logo"
      className="w-full h-full object-contain" // Ensures the image fits within the card
    />
  </div>

  {/* Card 2: Nutanix Logo */}
  <div className="w-80 h-44 p-4 rounded-lg  flex items-center justify-center">
    <img
      src="/assets/nutan.png" // Nutanix logo
      alt="Nutanix Logo"
      className="w-full h-full object-contain"
    />
  </div>

  {/* Cards 3 & 4: AIEA Lab & Tech4Good Logos */}
  <div className="flex space-x-4 justify-center">
    {/* Card 3: AIEA Lab Logo */}
    <div className="w-40 h-40 p-4 rounded-lg  flex items-center justify-center">
      <img
        src="/assets/aiea.png" // AIEA Lab logo
        alt="AIEA Lab Logo"
        className="w-full h-full object-contain"
      />
    </div>

    {/* Card 4: Tech4Good Logo */}
    <div className="w-40 h-40 p-4 rounded-lg flex items-center justify-center">
      <img
        src="/assets/t4.png" // Tech4Good logo
        alt="Tech4Good Logo"
        className="w-full h-full object-contain"
      />
    </div>
  </div>
</div>


    </section>
  );
};

export default Services;
