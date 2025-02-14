"use client";

import Link from "next/link";
import { usePathname } from "next/navigation";

const links = [
  {
    name: "home",
    path: "/",
  },
  {
    name: "about me",
    path: "#services",  // Changed to anchor links
  },
  {
    name: "resume",
    path: "#resume",  // Changed to anchor links
  },
  {
    name: "skills",
    path: "#skills",  // Changed to anchor links
  },
  {
    name: "work",
    path: "#work",  // Changed to anchor links
  },
  {
    name: "contact",
    path: "#contact",  // Changed to anchor links
  },
];

const Nav = () => {
  const pathname = usePathname();

  const scrollToSection = (event, id) => {
    event.preventDefault();
    const section = document.querySelector(id);
    section?.scrollIntoView({ behavior: "smooth", block: "start" });
  };

  return (
    <nav className="flex gap-8">
      {links.map((link, index) => {
        return (
          <a
            href={link.path}
            key={index}
            onClick={(e) => scrollToSection(e, link.path)}  // Trigger scroll on click
            className={`${
              link.path === pathname && "text-accent border-b-2 border-accent"
            } capitalize font-medium hover:text-accent transition-all`}
          >
            {link.name}
          </a>
        );
      })}
    </nav>
  );
};

export default Nav;
