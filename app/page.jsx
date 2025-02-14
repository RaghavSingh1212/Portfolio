"use client";

import { Button } from "@/components/ui/button";
import { FiDownload } from "react-icons/fi";
import { useEffect, useState } from "react"; // Use for typewriter effect
import Social from "@/components/Social";
import Photo from "@/components/Photo";
import Services from "@/app/services/page";
import Resume from "@/app/resume/page";
import Work from "@/app/work/page";
import Contact from "@/app/contact/page";
import Skills from "@/app/skills/page";
import Typewriter from "typewriter-effect"; // Import Typewriter library

const Home = () => {
  return (
    <div className="snap-container">
      {/* Home Section */}
      <section id="home" className="snap-start h-screen flex justify-center items-center text-white mt-[-50px] xl:mt-[-80px]">
        <div className="text-center xl:text-left">
          <h1 className="text-5xl xl:text-6xl font-bold mb-6">
            Hello I'm <br /> <span className="text-accent text-white/60">Raghav Singh</span>
          </h1>

          {/* Typewriter effect for Frontend Developer */}
          <span className="text-8xl xl:text-4xl mb-6 block typewriter-text">
            <Typewriter
              options={{
                strings: ["Frontend Developer"], // The text to be typed
                autoStart: true,
                loop: true, // Loop the effect
                deleteSpeed: 50, // Speed of deleting
              }}
            />
          </span>

          <p className="max-w-[500px] mb-9 text-white/80 text-xl">
            I'm a driven problem-solver with a passion for AI and software engineering, always seeking to innovate and deliver impactful, scalable solutions.
          </p>

          <div className="flex flex-col xl:flex-row items-center gap-8">
            {/* Resume link button */}
            <Button
              asChild
              variant="outline"
              size="lg"
              className="uppercase flex items-center gap-2"
            >
              <a href="https://drive.google.com/file/d/1Gy2msCuiYsZ7_9BntdhHQw_DEw4ZL8B7/view?usp=sharing">
                <span>View Resume</span>
              </a>
            </Button>

            <div className="mb-8 xl:mb-0">
              <Social containerStyles="flex gap-6" iconStyles="w-14 h-14 border border-accent rounded-full flex justify-center items-center text-accent text-base hover:bg-accent hover:text-primary hover:transition-all duration-500"/>
            </div>
          </div>
        </div>
        <div className="order-1 xl:order-none mb-8 xl:mb-0">
          <Photo />
        </div>
      </section>

      {/* Other Sections */}
      <section id="services"><Services /></section>
      <section id="resume"><Resume /></section>
      <section id="skills"><Skills /></section>
      <section id="work"><Work /></section>
      <section id="contact"><Contact /></section>
    </div>
  );
};

export default Home;
