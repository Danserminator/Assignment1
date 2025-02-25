# Assignment1
<h2><strong>Assignment 1 deals with motion models and path planning.</strong></h2>
<p>The objective of Assignment 1 is that you are familiar with the most important motion models used in multi agent systems, and understand how the different motion models impact a typical path planning problem.</p>
<p>Assignment 1 includes the following Tasks, that will be explained in more detail below</p>
<ul>
<li>T0 - Project Management including progress reporting</li>
<li>T1 - Waypoint following for the different motion models</li>
<li>T2 - Path planning for Discrete motion model</li>
<li>T3 - Path planning for Kinematic point model</li>
<li>T4 - Path planning for Dynamic point mass model</li>
<li>T5 - Path planning for Differential Drive model</li>
<li>T6 - Path planning for Kinematic Car model</li>
<li>T7 - Path planning for Dynamic Car model</li>
<li>T8 - Describe solution to T1-T7 in a scientific paper</li>
</ul>
<p>The path planning problem to be solved in Tasks 2-7 is the following:</p>
<p><strong>Find the minimum time path from A to B in a polygonal environment</strong></p>
<h2>Detailed Description</h2>
<p>The <strong>detailed description</strong> of Assignment 1 can be found here (<a href="https://www.kth.se/social/files/569a2401f2765408a576eca4/assignment1.pdf" title="pdf (pdf)">pdf</a>).</p>
<p>A description of the report where you describe your work can be found here (<a href="https://www.kth.se/social/course/DD2438/page/assignments-45/">link</a>).</p>
<h2>Example Problems</h2>
<p>Below you find a set of example problems. Solving these enables us to compare the performance of different algorithms on the course meetings.<br />Solve the problems, and <strong>report you current best solution on the course Wiki</strong>.</p>
<p><strong>Problem 1 (Discrete obstacle map)</strong><br />In these problems you will use the discrete motion models, and find the path with the smallest path length (with a diagonal step having sqrt(2) in path length).</p>
<p>The map, including start and endpoints are given in the data file below. <br /><a href="https://www.kth.se/social/files/54c8ef46f276546514fca31f/discObst.mat" title="discObst.mat (x-matlab-workspace)">discObst.mat</a> (the obstacle data)<br /><a href="https://www.kth.se/social/files/54c8ef69f276546c1569235a/plotDiscObst.m" title="plotDiscObst.m (octet-stream)">plotDiscObst.m</a> (m-file to import and plot the data)</p>
<p>Problem <strong>1a</strong>: Solve the problem using a 4-neighborhood<br />Problem <strong>1b</strong>: Solve the problem using a 8-neighborhood<br />Problem <strong>1c</strong>: Solve the problem using a 16-neighborhood</p>
<p><br /><strong>Problem 2 (Polygonal obstacle map)</strong><br />In these problems you will use the different continuous motion models,<br />searching for the minimum time path.</p>
<p>The map, including start and endpoints are given in the data file below. <br /><a href="https://www.kth.se/social/files/54c8efdef276546dbaa6e5b3/polygObst.mat" title="polygObst.mat (x-matlab-workspace)">polygObst.mat</a> (the obstacle data)<br /><a href="https://www.kth.se/social/files/54c8eff4f276546184d44266/plotPolygObst.m" title="plotPolygObst.m (octet-stream)">plotPolygObst.m</a> (m-file to import and plot the data)</p>
<p>Problem <strong>2a</strong>: Solve the problem using the Kinematic point model (Vmax=1)<br />Problem <strong>2b</strong>: Solve the problem using the Dynamic point model (Amax=0.4, Vmax=1)<br />Problem <strong>2c</strong>: Solve the problem using the Differential Drive model (Vmax=1,Wmax=1 rad/s)<br />Problem <strong>2d</strong>: Solve the problem using the Kinematic Car model (Vmax=1, Phimax=1 rad)<br />Problem <strong>2e</strong>: Solve the problem using the Dynamic Car model (Amax=0.1, Phimax=1 rad)</p>
<p></p>
<p>Problem <strong>3</strong>: Same as Problem 2 above, but with this map <a href="https://www.kth.se/social/files/54d874b1f276547654430ead/polygObst2.mat" title="polyObst2.mat (x-matlab-workspace)">polyObst2.mat</a>. An ascii-version of the data is found in this file <a href="https://www.kth.se/social/files/54d8ae04f2765457435c9eb0/polygObst2.txt" title="polyObst2.txt (plain)">polyObst2.txt</a> (to interpret it, look at the m-file that produced it, same as mat-file, but with &#39;-ascii&#39; option).</p>
<h2>Links</h2>
<p>Links to possibly (you decide) interesting material</p>
<ul>
<li><a href="http://en.wikipedia.org/wiki/Rapidly_exploring_random_tree">http://en.wikipedia.org/wiki/Rapidly_exploring_random_tree</a></li>
<li><a href="http://en.wikipedia.org/wiki/Visibility_graph">http://en.wikipedia.org/wiki/Visibility_graph</a><a href="http://en.wikipedia.org/wiki/Visibility_graph"></a></li>
<li>http://en.wikipedia.org/wiki/Voronoi_diagram</li>
<li><a href="http://sertac.scripts.mit.edu/web/wp-content/papercite-data/pdf/karaman.frazzoli-ijrr11.pdf">http://sertac.scripts.mit.edu/web/wp-content/papercite-data/pdf/karaman.frazzoli-ijrr11.pdf</a></li>
<li><a href="http://arl.cs.utah.edu/pubs/ICRA2013-1.pdf">http://arl.cs.utah.edu/pubs/ICRA2013-1.pdf</a></li>
<li><a href="http://repository.cmu.edu/cgi/viewcontent.cgi?article=1967&amp;context=robotics">http://repository.cmu.edu/cgi/viewcontent.cgi?article=1967&amp;context=robotics</a><a href="http://repository.cmu.edu/cgi/viewcontent.cgi?article=1967&amp;context=robotics"></a></li>
<li>http://arxiv.org/pdf/1404.2334v3.pdf</li>
</ul>
