"use client";

import { FaPhoneAlt, FaEnvelope, FaMapMarkerAlt, FaLinkedin } from "react-icons/fa";
import { motion } from "framer-motion";

const info = [
  {
    icon: <FaPhoneAlt />,
    title: "Phone",
    description: "(510) 738-8851",
  },
  {
    icon: <FaEnvelope />,
    title: "Email",
    description: "rsingh77@ucsc.edu",
  },
  {
    icon: <FaMapMarkerAlt />,
    title: "Address",
    description: "38764 Buckboard Common Fremont, CA",
  },
];

const Contact = () => {
  return (
    <motion.section
      id="contact"
      initial={{ opacity: 0 }}
      animate={{
        opacity: 1,
        transition: { delay: 2.4, duration: 0.4, ease: "easeIn" },
      }}
      className="py-12 mt-12"
    >
      <div className="container mx-auto">
        <div className="flex flex-col xl:flex-row gap-[30px] justify-center items-center">
          {/* Get in Touch Section */}
          <div className="text-center mb-12">
            <h2 className="text-5xl font-bold text-black mb-4">Contact Me :)</h2>
            <div className="flex justify-center gap-12 bg-transparent p-6 rounded-xl border-2 border-white/20">
              <div className="flex items-center gap-4 text-black/70">
                <div className="text-[28px]">
                  <FaEnvelope />
                </div>
                <a
                  href="mailto:rsingh77@ucsc.edu"
                  className="text-black hover:text-accent"
                >
                  rsingh77@ucsc.edu
                </a>
              </div>
              <div className="flex items-center gap-4 text-black/70">
                <div className="text-[28px]">
                  <FaLinkedin />
                </div>
                <a
                  href="https://www.linkedin.com/in/raghavsingh1212" // Replace with your actual LinkedIn URL
                  className="text-black hover:text-accent"
                  target="_blank"
                  rel="noopener noreferrer"
                >
                  LinkedIn
                </a>
              </div>
            </div>
          </div>
        </div>

        {/* Info Section with Icons (Phone, Email, Address) */}
        <div className="flex flex-col xl:flex-row gap-[30px] justify-center items-center mt-12">
          <ul className="flex gap-12">
            {info.map((item, index) => {
              return (
                <li key={index} className="flex items-center gap-6 text-white/77">
                  <div className="w-[52px] h-[52px] xl:w-[72px] xl:h-[72px] text-accent border border-white/20 rounded-md flex items-center justify-center">
                    <div className="text-[28px]">{item.icon}</div>
                  </div>
                  <div>
                    <p className="text-white/60">{item.title}</p>
                    <h3 className="text-xl">{item.description}</h3>
                  </div>
                </li>
              );
            })}
          </ul>
        </div>
      </div>

      {/* Footer */}
      <div className="text-center mt-12">
        <p className="text-white/60">
          Copyright © 2025 Raghav Singh. All Rights Reserved.
        </p>
      </div>
    </motion.section>
  );
};

export default Contact;
