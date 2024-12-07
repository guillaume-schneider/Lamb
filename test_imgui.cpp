static std::string componentName = "Default Name";
        static bool isRenaming = false; // État d'édition du nom
        static char nameBuffer[128];    // Buffer pour la saisie du nouveau nom

        // Your ImGui window and UI setup here
        // ImGui::Begin("Example Window");
        ImGui::Begin("Components Section");
        ImGui::Text("Hello, ImGui!");
        if (ImGui::BeginPopupContextWindow("ComponentPopup")) { // Unique name for popup
            if (ImGui::MenuItem("Add Transform Component")) {
                components.push_back(1);
            }
            if (ImGui::MenuItem("Add Render Component")) {
                // Code to create a Render component
            }
            if (ImGui::MenuItem("Add Physics Component")) {
                // Code to create a Physics component
            }
            ImGui::EndPopup();
        }
        ImGui::Text("Created Components:");
        for (size_t i = 0; i < components.size(); ++i) {
            ImGui::CollapsingHeader("Entity");
        }
        ImGui::End();

        if (!isRenaming) {
            ImGui::Text("%s", componentName.c_str());

            // Détecte un double-clic pour activer l'édition
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                isRenaming = true;
                strncpy(nameBuffer, componentName.c_str(), sizeof(nameBuffer)); // Initialise le buffer avec le nom actuel
            }
        }
        // Mode édition : affiche un champ de texte pour renommer le composant
        else {
            ImGui::SetKeyboardFocusHere(); // Place le focus sur le champ de texte
            if (ImGui::InputText("##edit", nameBuffer, sizeof(nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                componentName = nameBuffer; // Met à jour le nom du composant
                isRenaming = false;         // Quitte le mode édition
            }

            // Si l'utilisateur clique en dehors du champ, quitter le mode édition sans changer le nom
            if (!ImGui::IsItemActive() && !ImGui::IsItemHovered()) {
                isRenaming = false;
            }
        }

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Handle platform windows for multi-viewports
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();

            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();

            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }
