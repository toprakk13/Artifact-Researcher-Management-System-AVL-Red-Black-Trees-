
#include "ArtifactManager.h"
#include <iostream>
#include <sstream>

ArtifactManager::ArtifactManager()
{
}

ArtifactManager::~ArtifactManager()
{
}

int ArtifactManager::tokenize(const std::string &line, std::string tokens[], int maxTokens) const
{
    std::istringstream iss(line);
    std::string tok;
    int count = 0;
    while (iss >> tok && count < maxTokens)
    {
        tokens[count++] = tok;
    }
    return count;
}

void ArtifactManager::parseAndExecute(const std::string &line)
{
    // TODO: read lines and execuıte each command
    // Print "Error: Unknown command" if command is not known
    
    std::stringstream ss(line);
    std::string tokens[10];   
    int count = 0;

    while (ss >> tokens[count]) {
        count++;
    }

    std::string command = tokens[0];
    if(command=="#"|| count==0){return;} 
    if(command=="ADD_ARTIFACT"){
        handleAddArtifact(tokens,count);
    }
    else if(command=="HIRE_RESEARCHER"){
        handleHireResearcher(tokens,count);
    }
    else if(command=="REQUEST"){
        handleRequest(tokens,count);
    }
    else if(command=="RETURN"){
        handleReturn(tokens,count);
    }
    else if(command=="MATCH_RARITY"){
        handleMatchRarity(tokens,count);
    }
    else if(command=="RETURN_ALL"){
        handleReturnAll(tokens,count);
    }
    else if(command=="REMOVE_ARTIFACT"){
        handleRemoveArtifact(tokens,count);
    }
    else if(command=="FIRE_RESEARCHER"){
        handleFireResearcher(tokens,count);
    }
    else if(command=="RESEARCHER_LOAD"){
        handleResearcherLoad(tokens,count);
    }
    else if(command=="PRINT_UNASSIGNED"){
        handlePrintUnassigned(tokens,count);
    }
    else if(command=="PRINT_STATS"){
        handlePrintStats(tokens,count);
    }
    else if(command=="CLEAR"){
        handleClear(tokens,count);
    }
    else{
        std::cout << "Error: Unknown command"<<" '"<<command <<"'."<< std::endl;
    }
}

// =================== COMMAND HANDLERS ===================

void ArtifactManager::handleAddArtifact(const std::string tokens[], int count)
{
    // Expected: ADD_ARTIFACT <id> <name> <rarity> <value>
    // TODO:
    // 1) Check parameter count.
    // 2) Convert <id>, <rarity>, <value> to integers.
    // 3) Create Artifact and attempt to insert into AVL tree.
    // 4) On success: print "Artifact <id> added."
    // 5) On duplicate: print appropriate error (as in the PDF).
    int id = std::stoi(tokens[1]);
    std::string name = tokens[2];
    int rarity = std::stoi(tokens[3]);
    int value = std::stoi(tokens[4]);
    
    Artifact art(id, name, rarity, value);
    if(artifactTree.insertArtifact(art)){
        std::cout << "Artifact " << id << " added." << std::endl;
    } else {
        std::cout << "Error: Artifact " << id << " already exists." << std::endl;
    }
    
    
}

void ArtifactManager::handleRemoveArtifact(const std::string tokens[], int count)
{
    // Expected: REMOVE_ARTIFACT <id>
    // TODO:
    // 1) Parse id.
    // 2) Find artifact in AVL; if not found, print error.
    // 3) If artifact is assigned , find researcher and
    //    remove artifact from their list.
    // 4) Remove artifact from AVL; print success or error message.
    int id = std::stoi(tokens[1]);
    ArtifactNode *node = artifactTree.findArtifact(id);
    if(node == nullptr){
        std::cout << "Error: Artifact " << id << " not found." << std::endl;
    } else {
        Artifact &art = node->data;
        if(art.assignedToName != ""){
            ResearcherNode *rsc = researcherTree.findResearcher(art.assignedToName);
            if (rsc) {
                rsc->data.removeArtifact(id);
            }
        }
        
        if(artifactTree.removeArtifact(id)){
            std::cout << "Artifact " << id << " removed." << std::endl;
        }
    }

}

void ArtifactManager::handleHireResearcher(const std::string tokens[], int count)
{
    // Expected: HIRE_RESEARCHER <name> <capacity>
    // TODO:
    // 1) Parse name and capacity.
    // 2) Create Researcher and insert into RedBlackTree.
    // 3) On success: "Researcher <name> hired."
    // 4) On duplicate: error message.
    std::string name=tokens[1];
    int capa = std::stoi(tokens[2]);
    Researcher res(name,capa);
    if (researcherTree.insertResearcher(res)) {
        std::cout << "Researcher " << name << " hired." << std::endl;
    }
    else {
        std::cout << "Error: Researcher " << name << " already exists." << std::endl;
    }
    
    
}

void ArtifactManager::handleFireResearcher(const std::string tokens[], int count)
{
    // Expected: FIRE_RESEARCHER <name>
    // TODO:
    // 1) Find researcher by name. If not found, print error.
    // 2) For each artifact ID in their assignment list:
    //      - clear assignedToName in AVL.
    // 3) Remove researcher from RBT.
    // 4) Print success message.
    std::string name=tokens[1];
    ResearcherNode *rsc=researcherTree.findResearcher(name);
    
    if(rsc==nullptr){
        std::cout<<"Error: Researcher "<<name<<" not found."<<std::endl;
    }
    else{
        Researcher &resc=rsc->data;
        for(int i=0;i<resc.numAssigned;i++){
            int artId=resc.assignedArtifacts[i];
            artifactTree.clearAssignedTo(artId);
        }
        researcherTree.removeResearcher(name);
        std::cout << "Researcher " << name << " fired." << std::endl;
    }
}

void ArtifactManager::handleRequest(const std::string tokens[], int count)
{
    // Expected: REQUEST <researcherName> <artifactID>
    // TODO:
    // 1) Find researcher by name; error if missing.
    // 2) Find artifact by ID; error if missing.
    // 3) Check artifact is unassigned; error if already assigned.
    // 4) Check researcher capacity; error if at full capacity.
    // 5) On success: add artifact to researcher list AND set assignedToName in AVL.
    //    Print "Artifact <id> assigned to <name>."
    std::string name=tokens[1];
    int id = std::stoi(tokens[2]);
    ArtifactNode *p=artifactTree.findArtifact(id);
    ResearcherNode *x=researcherTree.findResearcher(name);
    Artifact &art= p->data;
    Researcher &rsc=x->data;
    if(x==nullptr){
        std::cout<<"Error: Researcher "<<name<<" not found."<<std::endl;
    }
    else if(p==nullptr){
        std::cout<<"Error: Artifact "<<id<<" not found."<<std::endl;
    }
    
    else if(!(art.assignedToName=="")){
            std::cout<<"Error: Artifact "<<id<<" already assigned."<<std::endl; 
    }
    
    else if(rsc.capacity<=rsc.numAssigned){
        std::cout<<"Error: Researcher "<<name<<" is at full capacity."<<std::endl;
    }
    else{
        rsc.addArtifact(id);
        artifactTree.setAssignedTo(id,name);
        art.updateValueBasedOnUsage();
        std::cout<<"Artifact "<<id<<" assigned to "<<name<<"."<<std::endl;
    }

    
}

void ArtifactManager::handleReturn(const std::string tokens[], int count)
{
    // Expected: RETURN <researcherName> <artifactID>
    // TODO:
    // 1) Validate existence of researcher and artifact.
    // 2) Check that artifact.assignedToName == researcherName.
    // 3) If not, print error.
    // 4) Otherwise, remove artifact from researcher list, clear assignedToName in AVL.
    //    Print "Artifact <id> returned by <name>."
    std::string name = tokens[1];
    int id = std::stoi(tokens[2]);
    
    ResearcherNode *p = researcherTree.findResearcher(name);
    ArtifactNode *x = artifactTree.findArtifact(id);

    if(p == nullptr){
        std::cout << "Error: Researcher " << name << " not found." << std::endl;
        return;
    }
    if(x == nullptr){
        std::cout << "Error: Artifact " << id << " not found." << std::endl;
        return;
    }

    Artifact &art = x->data;
    Researcher &rsc = p->data;

    if(art.assignedToName == name){
        rsc.removeArtifact(id);
        artifactTree.clearAssignedTo(id);
        std::cout << "Artifact " << id << " returned by " << name << "." << std::endl;
    } else {
        std::cout << "Error: Artifact " << id << " not assigned to " << name << "." << std::endl;
    }
    
}

void ArtifactManager::handleReturnAll(const std::string tokens[], int count)
{
    // Expected: RETURN_ALL <researcherName>
    // TODO:
    // 1) Find researcher; error if missing.
    // 2) For each artifact they supervise, clear assignedToName in AVL.
    // 3) Clear researcher's assignment list (removeAllArtifacts()).
    // 4) Print appropriate confirmation message.
    std::string name = tokens[1];
    ResearcherNode *p = researcherTree.findResearcher(name);
    if(p == nullptr){
        std::cout << "Error: Researcher " << name << " not found." << std::endl;
    } 
    else {
        Researcher &rsc = p->data; 
        for(int i = 0; i < rsc.numAssigned; i++) {
            int artID = rsc.assignedArtifacts[i];
            artifactTree.clearAssignedTo(artID); 
        }
        rsc.numAssigned = 0;
        std::cout << "All artifacts returned by " << name << "." << std::endl;
    }
}

void ArtifactManager::handleResearcherLoad(const std::string tokens[], int count)
{
    // Expected: RESEARCHER_LOAD <name>
    // TODO:
    // 1) Find researcher by name.
    // 2) If not found, print error.
    // 3) Otherwise, print number of supervised artifacts in required format.
    std::string name=tokens[1];
    ResearcherNode *p =researcherTree.findResearcher(name);
    if(p==nullptr){
        std::cout<<"Error: Researcher "<<name<<" not found."<<std::endl;
    }
    else{
        std::cout << "Researcher " << name << " is supervising " 
                  << p->data.numAssigned << " artifacts." << std::endl;
    }
}

void ArtifactManager::handleMatchRarity(const std::string tokens[], int count)
{
    // Expected: MATCH_RARITY <minRarity>
    // TODO:
    // Traverse AVL tree and print all artifacts with rarity >= minRarity.
    // You may choose any reasonable order (probably inorder) unless specified otherwise
    // in your PDF. Artifacts may be assigned or unassigned; print as required.
    int minRarity = std::stoi(tokens[1]);
    
    std::cout << "=== MATCH_RARITY " << minRarity << " ===" << std::endl;
    
    
    const int MAX_ID_CHECK = 2000; 
    for (int id = 0; id <= MAX_ID_CHECK; id++) {
        ArtifactNode *node = artifactTree.findArtifact(id);
        if (node != nullptr) {
            if (node->data.rarityLevel >= minRarity) {
                std::cout << node->data.artifactID << " " << node->data.name << " " 
                          << node->data.rarityLevel << " " << node->data.researchValue;
            if(node->data.assignedToName != "")
                std::cout << " ASSIGNED:" << node->data.assignedToName << std::endl;
                else 
                    std::cout << " UNASSIGNED" << std::endl;
        }
    }
    }
}

void ArtifactManager::handlePrintUnassigned(const std::string tokens[], int count)
{
    // Expected: PRINT_UNASSIGNED
    // TODO:
    // Print a header if needed, then call artifactTree.printUnassigned().
    std::cout<<"Unassigned artifacts:"<<std::endl;
    artifactTree.printUnassigned();
    
}

void ArtifactManager::handlePrintStats(const std::string tokens[], int count)
{
    // Expected: PRINT_STATS
    // TODO:
    // 1) Compute:
    //    - totalArtifacts (artifactTree.getArtifactCount())
    //    - totalResearchers (researcherTree.getResearcherCount())
    //    - average artifact rarity (floor of totalRarity / totalArtifacts)
    //    - average researcher load (floor of totalLoad / totalResearchers)
    // 2) Print summary lines exactly as in the spec.
    // 3) Then:
    //    - Print researchers using preorder traversal:
    //      researcherTree.traversePreOrderForStats()
    //    - Print artifacts using postorder traversal:
    //      artifactTree.traversePostOrderForStats()
    //    (Exact formatting defined in your PDF.)
    int totalArtifacts = artifactTree.getArtifactCount();
    int totalResearchers = researcherTree.getResearcherCount();
    int totalRaritySum = artifactTree.getTotalRarity();
    int totalLoadSum = researcherTree.getTotalLoad();
    int avgRarity = (totalArtifacts > 0) ? (totalRaritySum / totalArtifacts) : 0;
    int avgLoad = (totalResearchers > 0) ? (totalLoadSum / totalResearchers) : 0;
    std::cout << "=== SYSTEM STATISTICS ===" << std::endl;
    std::cout << "Artifacts: " << totalArtifacts << std::endl;
    std::cout << "Researchers: " << totalResearchers << std::endl;
    std::cout << "Average rarity: " << avgRarity << std::endl;
    std::cout << "Average load: " << avgLoad << std::endl;
    std::cout << "Researchers:"<<std::endl;
    researcherTree.traversePreOrderForStats();
    std::cout << "Artifacts:"<< std::endl;
    artifactTree.traversePostOrderForStats();
}

void ArtifactManager::handleClear(const std::string tokens[], int count)
{
    // Expected: CLEAR
    // TODO:
    // Clear both trees and print confirmation message.
    artifactTree.clear();
    researcherTree.clear();
    // e.g. "All data cleared."
    std::cout<<"All data cleared.";
}
